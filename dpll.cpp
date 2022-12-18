#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <utility>

using namespace std;
#include <chrono>
using namespace std::chrono;

void printClause(vector<int>& clause) {
    for (int term : clause) {
        cout << term << " ";
    }
}

void printClauses(vector<vector<int>>& clauses) {
    cout << "clauses: \n";
    for (vector<int> clause : clauses) {
        printClause(clause);
        cout << "\n";
    }
}

void printFrequencies(vector<pair<int, int>>& frequencies) {
    cout << "frequencies: \n";
    for (int i = 1; i < (int)frequencies.size(); i++) {
        pair<int, int> frequency = frequencies[i];
        cout << i << ": " << frequency.first << "," << frequency.second << "\n";
    }
}

bool checkClausesEmpty(vector<vector<int>>& clauses) {
    return clauses.size() == 0;
}

bool checkEmptyClause(vector<vector<int>>& clauses) {
    for (vector<int> clause : clauses) {
        if (clause.size() == 0) {
            return true;
        }
    }
    return false;
}

int checkPureLiteral(vector<pair<int, int>>& frequencies) {
    for (int i = 1; i < (int)frequencies.size(); i++) {
        pair<int, int> frequency = frequencies[i];
        if (frequency.first == 0 && frequency.second != 0) {
            return -i;
        }
        if (frequency.second == 0 && frequency.first != 0) {
            return i;
        }
    }

    return 0;
}

int checkUnitClause(vector<vector<int>>& clauses) {
    for (int i = 0; i < (int)clauses.size(); i++) {
        if (clauses[i].size() == 1) {
            return clauses[i][0];
        }
    }

    return 0;
}

inline void assignInClauses(vector<vector<int>>& clauses, vector<pair<int, int>>& frequencies, int trueLiteral) {
    int indexTrueLiteral = abs(trueLiteral);
    for (int i = (int)clauses.size() - 1; i >= 0; i--) {
        vector<int>& clause = clauses[i];
        for (int j = (int)clause.size() - 1; j >= 0; j--) {
            if (clause[j] == trueLiteral) {
                for (int clauseLiteral : clauses[i]) {
                    int indexClauseLiteral = abs(clauseLiteral);
                    if (clauseLiteral > 0) {
                        frequencies[indexClauseLiteral].first -= 1;
                    }
                    else {
                        frequencies[indexClauseLiteral].second -= 1;
                    }
                }
                clauses.erase(clauses.begin() + i);
                break;
            }
            if (clause[j] == -trueLiteral) {
                if (trueLiteral > 0) {
                    frequencies[indexTrueLiteral].second -= 1;
                }
                else {
                    frequencies[indexTrueLiteral].first -= 1;
                }
                clause.erase(clause.begin() + j);
            }
        }
    }
}

int chooseLiteral(vector<pair<int, int>> frequencies) {
    int maxLiteral = 0;
    int maxFrequency = -1;
    for (int i = 1; i < frequencies.size(); i++) {
        pair<int, int> frequency = frequencies[i];
        if (frequency.first > maxFrequency) {
            maxLiteral = i;
            maxFrequency = frequency.first;
        }
        if (frequency.second > maxFrequency) {
            maxLiteral = -i;
            maxFrequency = frequency.second;
        }
    }
    return maxLiteral;
}

bool DPLL(vector<vector<int>>& clauses, vector<pair<int, int>>& frequencies) {
    if (checkClausesEmpty(clauses)) {
        return true;
    }

    if (checkEmptyClause(clauses)) {
        return false;
    }

    int pureLiteral = checkPureLiteral(frequencies);
    while (pureLiteral != 0) {
        assignInClauses(clauses, frequencies, pureLiteral);
        pureLiteral = checkPureLiteral(frequencies);
    }

    int unitClauseLiteral = checkUnitClause(clauses);
    while (unitClauseLiteral != 0) {
        assignInClauses(clauses, frequencies, unitClauseLiteral);
        unitClauseLiteral = checkUnitClause(clauses);
    }

    int chosenLiteral = chooseLiteral(frequencies);
    vector<vector<int>> secondClauses = clauses;
    vector<pair<int, int>> secondFrequencies = frequencies;

    assignInClauses(clauses, frequencies, chosenLiteral);
    assignInClauses(secondClauses, secondFrequencies, -chosenLiteral);

    return DPLL(clauses, frequencies) || DPLL(secondClauses, secondFrequencies);
}

bool basicDPLL() {
    /*
    * read sat problem into program
    */

    std::string clauseTxt;
    std::ifstream clausesFile;

    /*cout << "Enter file path:";
    string filePath;
    cin >> filePath;
    clausesFile.open(filePath);*/

    clausesFile.open("D:\\basic_cnf\\uf20-01.cnf");

    if (!clausesFile) {
        std::cout << "Invalid file path.";
        return false;
    }

    vector<vector<int>> clauses;
    std::vector<std::pair<int, int>> frequencies;
    int numVariables = 0;
    int numClauses = 0;
    if (clausesFile.is_open()) {
        // handles header/comments logic
        while (true) {
            if (getline(clausesFile, clauseTxt)) {
                std::string tmp;
                std::istringstream clauseHeaderStream(clauseTxt);

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
                    std::cout << "Parsing beginning error.\n";
                    return false;
                }
            }
            else {
                std::cout << "Parsing beginning error.\n";
                return false;
            }
        }

        frequencies = std::vector<std::pair<int, int>>(numVariables + 1, std::make_pair(0, 0));
        while (getline(clausesFile, clauseTxt)) {
            // end of cnf indicator
            if (clauseTxt == "%") {
                break;
            }

            std::istringstream clauseTxtStream(clauseTxt);
            std::vector<int> clause;
            int term;

            while (clauseTxtStream >> term) {
                if (term != 0) {
                    clause.push_back(term);
                    if (term > 0) {
                        frequencies[abs(term)].first += 1;
                    }
                    else {
                        frequencies[abs(term)].second += 1;
                    }
                }
            }

            clauses.push_back(clause);
        }

        clausesFile.close();
    }

    std::cout << "number of variables: " << numVariables << "\n";
    std::cout << "number of clauses: " << numClauses << "\n";

    return DPLL(clauses, frequencies);
}

int main()
{
    auto start = high_resolution_clock::now();
    if (basicDPLL() == true) {
        std::cout << "SAT\n";
    }
    else {
        std::cout << "UNSAT\n";
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    std::cout << duration.count() << "\n";

    return 0;
}