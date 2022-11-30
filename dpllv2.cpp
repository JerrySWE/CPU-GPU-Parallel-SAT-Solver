#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <sstream>
#include <map>
#include <utility>
#include <future>
#include <algorithm>
#include <mutex>
#include <random>


#include <chrono>
using namespace std::chrono;

void printClause(const std::list<int>& clause) {
    for (auto const& term : clause) {
        std::cout << term << " ";
    }
}

void printClauses(const std::list<std::list<int>>& clauses) {
    std::cout << "clauses: \n";
    for (auto const& clause : clauses) {
        printClause(clause);
        std::cout << "\n";
    }
}

void printFrequencies(const std::vector<std::pair<int, int>>& frequencies) {
    std::cout << "frequencies: \n";
    for (int i = 1; i < (int)frequencies.size(); i++) {
        const std::pair<int, int> frequency = frequencies[i];
        std::cout << i << ": " << frequency.first << "," << frequency.second << "\n";
    }
}

bool checkClausesEmpty(const std::list<std::list<int>>& clauses) {
    return clauses.size() == 0;
}

bool checkEmptyClause(const std::list<std::list<int>>& clauses) {
    for (auto const& clause : clauses) {
        if (clause.size() == 0) {
            return true;
        }
    }
    return false;
}

int checkPureLiteral(const std::vector<std::pair<int, int>>& frequencies) {
    for (int i = 1; i < (int)frequencies.size(); i++) {
        const std::pair<int, int> frequency = frequencies[i];
        if (frequency.first == 0 && frequency.second != 0) {
            return -i;
        }
        if (frequency.second == 0 && frequency.first != 0) {
            return i;
        }
    }

    return 0;
}

int checkUnitClause(std::list<std::list<int>>& clauses) {
    for (std::list<std::list<int>>::iterator it = clauses.begin(); it != clauses.end(); it++) {
        if (it->size() == 1) {
            return *(it->begin());
        }
    }
    return 0;
}

inline void assignInClauses(std::list<std::list<int>>& clauses, std::vector<std::pair<int, int>>& frequencies, int trueLiteral) {
    int indexTrueLiteral = abs(trueLiteral);
    std::list<std::list<int>>::iterator it = clauses.begin();
    bool didRemove = true;
    while (it != clauses.end()) {
        std::list<int>::iterator jt = it->begin();
        while (jt != it->end()) {
            if (*jt == trueLiteral) {
                for (int clauseLiteral : *it) {
                    int indexClauseLiteral = abs(clauseLiteral);
                    if (clauseLiteral > 0) {
                        frequencies[indexClauseLiteral].first -= 1;
                    }
                    else {
                        frequencies[indexClauseLiteral].second -= 1;
                    }
                }
                it = clauses.erase(it);
                didRemove = true;
                break;
            }
            if (*jt == -trueLiteral) {
                if (trueLiteral > 0) {
                    frequencies[indexTrueLiteral].second -= 1;
                }
                else {
                    frequencies[indexTrueLiteral].first -= 1;
                }
                jt = (*it).erase(jt);
            }
            else {
                jt++;
            }
        }
        if (!didRemove) {
            it++;
        }
        didRemove = false;
    }
}

int chooseLiteral(std::vector<std::pair<int, int>> frequencies) {
    int maxLiteral = 0;
    int maxFrequency = -1;

    #pragma omp simd
    for (int i = 1; i < frequencies.size(); i++) {
        std::pair<int, int> frequency = frequencies[i];
        if (frequency.first > maxFrequency) {
            maxLiteral = i;
            maxFrequency = frequency.first + frequency.second;
        }
    }

    return maxLiteral;
}

bool DPLL(std::list<std::list<int>>& clauses, std::vector<std::pair<int, int>>& frequencies, const int threadID) {
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
    std::list<std::list<int>> secondClauses = clauses;
    std::vector<std::pair<int, int>> secondFrequencies = frequencies;

    assignInClauses(clauses, frequencies, chosenLiteral);
    assignInClauses(secondClauses, secondFrequencies, -chosenLiteral);

    return DPLL(clauses, frequencies, threadID) || DPLL(secondClauses, secondFrequencies, threadID);
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

    clausesFile.open("D:\\basic_cnf\\uuf75-09.cnf");

    if (!clausesFile) {
        std::cout << "Invalid file path.";
        return false;
    }

    std::list<std::list<int>> clauses;
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
            std::list<int> clause;
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

    return DPLL(clauses, frequencies, 0);
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