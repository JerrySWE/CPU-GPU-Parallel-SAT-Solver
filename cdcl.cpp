#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <sstream>
#include <map>
#include <utility>
#include <future>
#include <algorithm>
#include <mutex>
#include <random>
#include <chrono>
using namespace std;

int decisionLevel = -1;

vector<pair<bool, vector<int>>> originalClauses;
vector<pair<bool, vector<int>>> clauses;
vector<pair<int, int>> frequencies;

struct literalState {
    char assignment;
    int decisionLevel;
    int antecedentClauseIndex;
};
vector<literalState> literalStates;

void print(string ptext) {
    cout << ptext << "\n";
}

void printAssignments() {
    for (int i = 1; i < literalStates.size(); i++) {
        cout << i << ": " << (int) (literalStates[i].assignment) << " at level " << literalStates[i].decisionLevel << "\n";
    }
}

void printClause(const pair<bool, vector<int>>& clause) {
    for (auto const& term : clause.second) {
        cout << term << " ";
    }
}

void printClauses() {
    cout << "clauses: \n";
    for (auto const& clause : clauses) {
        for (auto const& term : clause.second) {
            cout << term << " ";
        }
        cout << "\n";
    }
}

void printFrequencies() {
    cout << "frequencies: \n";
    for (size_t i = 1; i < frequencies.size(); i++) {
        const pair<int, int> frequency = frequencies[i];
        cout << i << ": " << frequency.first << "," << frequency.second << "\n";
    }
}

bool allAssigned() {
    for (int i = 1; i < literalStates.size(); i++) {
        if (literalStates[i].assignment == 0) {
            return false;
        }
    }
    return true;
}

pair<bool, int> checkEmptyClause() {
    for (int i = 0; i < clauses.size(); i++) {
        if (clauses[i].second.size() == 0) {
            return make_pair(true, i);
        }
    }

    return make_pair(false, -1);
}

pair<int, int> findUnitClause() {
    for (int i = 0; i < clauses.size(); i++) {
        if (clauses[i].first != true) {
            int lastUnassigned = 0;
            int numUnassigned = 0;
            int numFalsified = 0;

            for (auto term : clauses[i].second) {
                int assignedValue = abs(term) * literalStates[abs(term)].assignment;
                if (term == assignedValue) {
                    clauses[i].first = true;
                    break;
                }
                if (term == -assignedValue) {
                    numFalsified += 1;
                }
                else {
                    numUnassigned += 1;
                    lastUnassigned = term;
                }
            }

            if (numUnassigned == 1 && ((numUnassigned + numFalsified) == clauses[i].second.size())) {
                return make_pair(lastUnassigned, i);
            }
        }
    }

    return make_pair(0, -1);
}

int propagate() {
    pair<int, int> unitClauseLiteral = findUnitClause();
    while (unitClauseLiteral.first != 0) {
        literalStates[abs(unitClauseLiteral.first)].decisionLevel = decisionLevel;
        literalStates[abs(unitClauseLiteral.first)].assignment = (unitClauseLiteral.first > 0) - (unitClauseLiteral.first < 0);
        literalStates[abs(unitClauseLiteral.first)].antecedentClauseIndex = unitClauseLiteral.second;

        unitClauseLiteral = findUnitClause();
    }

    pair<bool, int> emptyState = checkEmptyClause();
    if (emptyState.first) {
        return -emptyState.second;
    }

    return 1;
}

int chooseLiteral() {
    int maxLiteral = 0;
    int maxFrequency = -1;

    for (int i = 1; i < (int)frequencies.size(); i++) {
        pair<int, int> frequency = frequencies[i];
        if (literalStates[i].assignment == 0) {
            if (frequency.first > maxFrequency) {
                maxLiteral = -i;
                maxFrequency = frequency.first;
            }
            if (frequency.second > maxFrequency) {
                maxLiteral = i;
                maxFrequency = frequency.second;
            }
        }
    }

    return maxLiteral;
}

int numberOfLevelLiterals(unordered_set<int> clause) {
    int levelLiterals = 0;
    for (auto term : clause) {
        if (literalStates[abs(term)].decisionLevel == decisionLevel) {
            levelLiterals += 1;
        }
    }
    return levelLiterals;
}

int analyzeConflict(int conflictClauseIndex) {
    unordered_set<int> conflictClause;
    for (auto startingTerm : originalClauses[conflictClauseIndex].second) {
        conflictClause.insert(startingTerm);
    }

    while (numberOfLevelLiterals(conflictClause) != 1) {
        for (auto term : conflictClause) {
            if (literalStates[abs(term)].decisionLevel == decisionLevel) {
                for (auto addingTerm : originalClauses[literalStates[abs(term)].antecedentClauseIndex].second) {
                    conflictClause.insert(addingTerm);
                }
                conflictClause.erase(term);
                conflictClause.erase(-term);
                continue;
            }
        }
    }

    vector<int> newClause;
    newClause.assign(conflictClause.begin(), conflictClause.end());
    clauses.push_back(make_pair(false, newClause));

    int secondMaxTerm = -1;
    int secondMaxLevel = 0;

    for (auto newTerm : newClause) {
        if (newTerm > 0) {
            frequencies[abs(newTerm)].first += 1;
        }
        else {
            frequencies[abs(newTerm)].second += 1;
        }
        if (literalStates[abs(newTerm)].decisionLevel > secondMaxLevel && literalStates[abs(newTerm)].decisionLevel != decisionLevel) {
            secondMaxTerm = abs(newTerm);
            secondMaxLevel = literalStates[abs(newTerm)].decisionLevel;
        }
    }

    decisionLevel = secondMaxLevel;
}

bool CDCL(const int threadID) {
    int initialPropagation = propagate();
    
    if (initialPropagation <= 0) {
        return false;
    }

    if (initialPropagation == 2) {
        return true;
    }

    decisionLevel = 0;

    while (!allAssigned()) {
        int chosenLiteral = chooseLiteral();

        decisionLevel++;

        literalStates[abs(chosenLiteral)].decisionLevel = decisionLevel;
        literalStates[abs(chosenLiteral)].assignment = (chosenLiteral > 0) - (chosenLiteral < 0);
        literalStates[abs(chosenLiteral)].antecedentClauseIndex = -1;

        int loopPropagation = propagate();

        if (loopPropagation <= 0) {
            decisionLevel = analyzeConflict(-loopPropagation);

            if (decisionLevel < 0) {
                return false;
            }

            for (int i = 1; i < literalStates.size(); i++) {
                if (literalStates[i].decisionLevel > decisionLevel) {
                    literalStates[i] = { 0, -2, -1 };
                }
            }
        }
    }

    return true;
}

bool basicCDCL() {
    auto startParse = chrono::high_resolution_clock::now();

    /*
    * read sat problem into program
    */

    string clauseTxt;
    ifstream clausesFile;

    /*cout << "Enter file path:";
    string filePath;
    cin >> filePath;
    clausesFile.open(filePath);*/

    clausesFile.open("D:\\basic_cnf\\uf20-01.cnf");

    if (!clausesFile) {
        cout << "Invalid file path.";
        return false;
    }

    int numVariables = 0;
    int numClauses = 0;
    if (clausesFile.is_open()) {
        // handles header/comments logic
        while (true) {
            if (getline(clausesFile, clauseTxt)) {
                string tmp;
                istringstream clauseHeaderStream(clauseTxt);

                clauseHeaderStream >> tmp;
                if (tmp == "c") {
                    continue;
                }
                else if (tmp == "p") {
                    clauseHeaderStream >> tmp;
                    clauseHeaderStream >> numVariables;
                    clauseHeaderStream >> numClauses;
                    break;
                }
                else {
                    cout << "Parsing beginning error.\n";
                    return false;
                }
            }
            else {
                cout << "Parsing beginning error.\n";
                return false;
            }
        }

        clauses.clear();
        frequencies = vector<pair<int, int>>(numVariables + 1, make_pair(0, 0));

        while (getline(clausesFile, clauseTxt)) {
            // end of cnf indicator
            if (clauseTxt == "%") {
                break;
            }

            istringstream clauseTxtStream(clauseTxt);
            vector<int> clause;
            int term;

            while (clauseTxtStream >> term) {
                if (term != 0) {
                    if (find(clause.begin(), clause.end(), term) == clause.end()) {
                        clause.push_back(term);
                        if (term > 0) {
                            frequencies[abs(term)].first += 1;
                        }
                        else {
                            frequencies[abs(term)].second += 1;
                        }
                    }
                }
            }

            clauses.push_back(make_pair(false, clause));
        }

        clausesFile.close();
    }

    cout << "number of variables: " << numVariables << "\n";
    cout << "number of clauses: " << numClauses << "\n";

    literalStates = vector<literalState>(numVariables + 1, { 0, -2, -1 });
    originalClauses = clauses;

    auto stopParse = chrono::high_resolution_clock::now();
    auto durationParse = chrono::duration_cast<chrono::milliseconds>(stopParse - startParse);
    cout << durationParse.count() << "\n";

    return CDCL(0);
}

int main()
{
    auto start = chrono::high_resolution_clock::now();

    if (basicCDCL() == true) {
        cout << "SAT\n";
    }
    else {
        cout << "UNSAT\n";
    }

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    cout << duration.count() << "\n";

    return 0;
}