#include <iostream>
#include <fstream>
#include <unistd.h>
#include <chrono>
#include "BoolSet.h"
#include "BigIntRepr.h"
#include <bits/stdc++.h>

bool solve(BigIntRepr &curr_assignment, std::vector<int> &CNF) {
    int c1, c2, c3;
    for (int i = 0; i < CNF.size(); i+=3) {
        c1 = CNF[i];
        c2 = CNF[i+1];
        c3 = CNF[i+2];

        if ((c1 > 0 && curr_assignment.getAssignment(c1)) || 
            (c2 > 0 && curr_assignment.getAssignment(c2)) || 
            (c3 > 0 && curr_assignment.getAssignment(c3)) ||
            (c1 < 0 && !curr_assignment.getAssignment(std::abs(c1))) ||
            (c2 < 0 && !curr_assignment.getAssignment(std::abs(c2))) ||
            (c3 < 0 && !curr_assignment.getAssignment(std::abs(c3)))) {
                continue;
        }

        return false;
    }

    return true;
}

void bruteForce(char *input_file_path, char *output_file_path) {
    std::ifstream infile(input_file_path);
    std::string line;
    //std::getline(infile, line);

    int vCount, cCount;
    infile >> vCount >> cCount;

    printf("vCount: %d\n", vCount);

    int c1, c2, c3;

    std::vector<int> CNF;

    while (infile >> c1 >> c2 >> c3) {
        CNF.emplace_back(c1);
        CNF.emplace_back(c2);
        CNF.emplace_back(c3);
    }

    auto bool_assign = BigIntRepr(vCount);

    auto start = std::chrono::high_resolution_clock::now();
    auto stop = std::chrono::high_resolution_clock::now();

    while (true) {
        if (solve(bool_assign, CNF)) {
            stop = std::chrono::high_resolution_clock::now();
            printf("Solvable!\n");
            std::ofstream outputFile(output_file_path);
            for (int i = 0; i < vCount; i++) {
                if (bool_assign.getAssignment(i)) {
                    outputFile << "Variable: " << std::to_string(i) << " assigned true" << std::endl;
                } else {
                    outputFile << "Variable: " << std::to_string(i) << " assigned false" << std::endl;
                }
            }
            outputFile.close();
            break;
        } else {
            if (!bool_assign.incrementSet()) {
                stop = std::chrono::high_resolution_clock::now();
                printf("unsolvable!\n");
                break;
            }
        }
    }

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop-start);
    std::cout << "Execution time: " << duration.count() << " ms" << std::endl;   
}

int main(int argc, char**argv) {

    int option;
    option = getopt(argc, argv, "i:o:");
    char *input_file_path;
    char *output_file_path;

    while (option != -1) {
        switch (option) {
            case 'i':
                input_file_path = optarg;
                break;
            case 'o':
                output_file_path = optarg;
                break;
        }
        option = getopt(argc, argv, "i:o:");
    }

    bruteForce(input_file_path, output_file_path);

    return 0;
}