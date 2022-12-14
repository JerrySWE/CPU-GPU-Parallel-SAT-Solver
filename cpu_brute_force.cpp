#include <iostream>
#include <fstream>
#include <unistd.h>
#include <chrono>
#include "BoolSet.h"
#include "BigIntRepr.h"
#include <bits/stdc++.h>

void readFile(char *input_file_path, std::vector<int> &CNF, int &v_count, int &c_count) {
    std::ifstream infile(input_file_path);

    infile >> v_count >> c_count;

    printf("v_count: %d\n", v_count);

    int c1, c2, c3;

    while (infile >> c1 >> c2 >> c3) {
        CNF.emplace_back(c1);
        CNF.emplace_back(c2);
        CNF.emplace_back(c3);
    }
}

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

void solveThreadBlock(std::vector<int> &CNF, int v_count, int c_count, char *output_file_path, size_t thread_id, int thread_count, bool *flag) {

    int bit_count = 0;

    int thread_assignment = thread_count - 1;

    while (thread_assignment > 0) {
        bit_count++;
        thread_assignment /= 2;
    }

    size_t shift_amount = v_count - bit_count;
    size_t thread_start = thread_id << shift_amount;

    size_t thread_search_space = ((size_t)1) << shift_amount;

    auto bool_assign = BigIntRepr(v_count);
    bool_assign.setStart(thread_start);

    auto start = std::chrono::high_resolution_clock::now();
    auto stop = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < thread_search_space; i++) {
        if (*flag) {
            std::cout << "Solved by other thread!\n";
            break;
        }

        if (solve(bool_assign, CNF)) {
            std::cout << "Solved by thread " << thread_id << std::endl;
            *flag = true;
            std::ofstream outputFile(output_file_path);
            for (int i = 0; i < v_count; i++) {
                if (bool_assign.getAssignment(i)) {
                    outputFile << "Variable: " << std::to_string(i) << " assigned true" << std::endl;
                } else {
                    outputFile << "Variable: " << std::to_string(i) << " assigned false" << std::endl;
                }
            }
            outputFile.close();
            break;
        }

        bool_assign.incrementSet();
    }
    stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop-start);
    std::cout << "Thread " << thread_id << " execution time " << duration.count() << " ms\n"; 
}

void bruteForceSeq(char *input_file_path, char *output_file_path) {
    int v_count, c_count;
    std::vector<int> CNF;

    readFile(input_file_path, CNF, v_count, c_count);

    auto bool_assign = BigIntRepr(v_count);

    auto start = std::chrono::high_resolution_clock::now();
    auto stop = std::chrono::high_resolution_clock::now();

    while (true) {
        if (solve(bool_assign, CNF)) {
            stop = std::chrono::high_resolution_clock::now();
            printf("Solvable!\n");
            std::ofstream outputFile(output_file_path);
            for (int i = 0; i < v_count; i++) {
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

void bruteForceThreadParallel(char *input_file_path, char *output_file_path, int thread_count) {
    // Main thread will read in the data
    int v_count, c_count;
    std::vector<int> CNF;
    readFile(input_file_path, CNF, v_count, c_count);

    std::vector<std::thread> threads;
    bool flag = false;
    auto start = std::chrono::high_resolution_clock::now();
    auto stop = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < thread_count; i++) {
        threads.emplace_back(std::thread(solveThreadBlock, std::ref(CNF), v_count, c_count, output_file_path, i, thread_count, &flag));
    }

    for (auto &t : threads) {
        t.join();
    }

    if (flag) {
        std::cout << "SOLVABLE!\n";
    } else {
        std::cout << "UNSOLVABLE!\n";
    }

    stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop-start);
    std::cout << "Execution time: " << duration.count() << " ms" << std::endl;   
}

int main(int argc, char**argv) {

    int option;
    option = getopt(argc, argv, "i:o:p:");
    char *input_file_path;
    char *output_file_path;
    
    int thread_count = 0;
    bool thread_parallel = false;

    while (option != -1) {
        switch (option) {
            case 'i':
                input_file_path = optarg;
                break;
            case 'o':
                output_file_path = optarg;
                break;
            case 'p':
                thread_count = std::stoi(optarg);
                thread_parallel = true;
                break;
        }
        option = getopt(argc, argv, "i:o:p:");
    }

    if (thread_parallel) {
        std::cout << "Parallel execution!" << std::endl;
        bruteForceThreadParallel(input_file_path, output_file_path, thread_count);
    } else {
        bruteForceSeq(input_file_path, output_file_path);
    }


    return 0;
}