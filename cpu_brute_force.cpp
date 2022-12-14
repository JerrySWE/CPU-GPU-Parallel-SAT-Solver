#include <iostream>
#include <fstream>
#include <unistd.h>
#include <chrono>
#include "BoolSet.h"
#include "BigIntRepr.h"
#include <bits/stdc++.h>

void readFileLegacy(char *input_file_path, std::vector<int> &CNF, int &v_count, int &c_count) {
    std::ifstream infile(input_file_path);

    infile >> v_count >> c_count;

    printf("v_count: %d\n", v_count);

    int c1, c2, c3;

    while (infile >> c1 >> c2 >> c3) {
        CNF.emplace_back(c1);
        CNF.emplace_back(c2);
        CNF.emplace_back(c3);
    }

    for (int i = 0; i < CNF.size(); i += 3) {
        std::cout << CNF[i] << " " << CNF[i+1] << " " << CNF[i+2] << std::endl;
    }
}

bool solveLegacy(BigIntRepr &curr_assignment, std::vector<int> &CNF) {
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

void solveThreadBlockLegacy(std::vector<int> &CNF, int v_count, int c_count, char *output_file_path, size_t thread_id, int thread_count, bool *flag) {

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

        if (solveLegacy(bool_assign, CNF)) {
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

void bruteForceSeqLegacy(char *input_file_path, char *output_file_path) {
    int v_count, c_count;
    std::vector<int> CNF;

    readFileLegacy(input_file_path, CNF, v_count, c_count);

    auto bool_assign = BigIntRepr(v_count);

    auto start = std::chrono::high_resolution_clock::now();
    auto stop = std::chrono::high_resolution_clock::now();

    while (true) {
        if (solveLegacy(bool_assign, CNF)) {
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

void bruteForceThreadParallelLegacy(char *input_file_path, char *output_file_path, int thread_count) {
    // Main thread will read in the data
    int v_count, c_count;
    std::vector<int> CNF;
    readFileLegacy(input_file_path, CNF, v_count, c_count);

    std::vector<std::thread> threads;
    bool flag = false;
    auto start = std::chrono::high_resolution_clock::now();
    auto stop = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < thread_count; i++) {
        threads.emplace_back(std::thread(solveThreadBlockLegacy, std::ref(CNF), v_count, c_count, output_file_path, i, thread_count, &flag));
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

void readFile(char *input_file_path, std::vector<int> &CNF, int &v_count) {
    std::ifstream infile(input_file_path);
    int c_count;

    infile >> v_count >> c_count;

    printf("v_count: %d\n", v_count);

    int c1, c2, c3;

    while (infile >> c1 >> c2 >> c3) {
        CNF.emplace_back(c1);
        CNF.emplace_back(c2);
        CNF.emplace_back(c3);
    }

    for (int i = 0; i < CNF.size(); i += 3) {
        std::cout << CNF[i] << " " << CNF[i+1] << " " << CNF[i+2] << std::endl;
    }
}

/*
 * Assignment is represented by unsigned 64 bits integer, each bit will represent the boolean value of a variable
 * Variable should range from 1-variable count (inclusive)
 * Note: This representation only support up to 64 boolean variables
 */
bool getAssignment(uint64_t assignment, int variable) {
    // The bit position of the variable in the assignment should be (the absolute value of the variable) - 1 due to 
    // 0 indexing
    size_t position = std::abs(variable) - 1;
    if (variable > 0) {
        return ((assignment >> position) & 0x1) == 1;
    } else {
        return ((assignment >> position) & 0x1) == 0;
    }
}

void writeAssignment(char *output_file_path, uint64_t assignment, size_t v_count) {
    std::ofstream outputFile(output_file_path);
    for (int i = 1; i < v_count+1; i++) {
        if (getAssignment(assignment, i)) {
            outputFile << "Variable: " << std::to_string(i) << " true" << std::endl;
        } else {
            outputFile << "Variable: " << std::to_string(i) << " false" << std::endl;
        }
    }
    outputFile.close();
}

bool solve(uint64_t assignment, std::vector<int> &CNF) {
    for (int i = 0; i < CNF.size(); i+=3) {
        int c1 = CNF[i];
        int c2 = CNF[i+1];
        int c3 = CNF[i+2];

        if (getAssignment(assignment, c1) || getAssignment(assignment, c2) || getAssignment(assignment, c3)) {
            continue;
        }

        return false;
    }

    return true;
}

void solveSeq(std::vector<int> &CNF, size_t v_count, char *output_file_path) {
    uint64_t base = 1;
    uint64_t search_space = base << v_count; 
    for (uint64_t assignment = 0; assignment < search_space; assignment++) {
        if (solve(assignment, CNF)) {
            std::cout << "CNF Solved!" << std::endl;
            writeAssignment(output_file_path, assignment, v_count);
            return;
        }
    }
    std::cout << "CNF unsolvable!" << std::endl;
    return;
}

void solveThreadBlock(std::vector<int> &CNF, size_t v_count, size_t c_count, char *output_file_path, size_t thread_id, size_t thread_count, bool *flag) {
    uint64_t base = 1;
    uint64_t search_space = base << v_count; 
    uint64_t thread_search_space = search_space / thread_count;
    uint64_t thread_start = thread_id * thread_search_space;

    for (uint64_t assignment = thread_start; assignment < thread_start + thread_search_space; assignment++) {

    }
}

void solveThreadInterleave(std::vector<int> &CNF, int v_count, int c_count, char *output_file_path, size_t thread_id, size_t thread_count, bool *flag) {
    uint64_t base = 1;
    uint64_t search_space = base << v_count; 
    uint64_t thread_search_space = search_space / thread_count;

    for (uint64_t assignment = thread_id; assignment < search_space; assignment+=thread_count) {
        
    }
}

void solveThreadCentralQueue(std::vector<int> &CNF, int v_count, int c_count, char *output_file_path, size_t thread_id, size_t thread_count, bool *flag) {

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

    std::vector<int> CNF;
    int v_count;
    readFile(input_file_path, CNF, v_count);

    if (thread_parallel) {
        std::cout << "Parallel execution!" << std::endl;
        // bruteForceThreadParallel(input_file_path, output_file_path, thread_count);
    } else {
        solveSeq(CNF, v_count, output_file_path);
    }


    return 0;
}