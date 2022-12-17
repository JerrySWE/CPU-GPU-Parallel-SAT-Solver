#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <getopt.h>
#include <string>

void SATCuda(int* CNF, int v_count, int c_count);
void printCudaInfo();

void usage(const char* progname) {
    printf("Usage: %s [options]\n", progname);
    printf("Program Options:\n");
    printf("  -i  Input test case path\n");
    printf("  -o  Output test case path\n");
}

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

int main(int argc, char** argv)
{
    int option;
    option = getopt(argc, argv, "i:o:");
    char *input_file_path = nullptr;
    char *output_file_path = nullptr;

    while (option != -1) {
        switch (option) {
            case 'i':
                input_file_path = optarg;
                break;
            case 'o':
                output_file_path = optarg;
                break;
            default:
                usage(argv[0]);
                return 1;
        }
        option = getopt(argc, argv, "i:o:");
    }

    int v_count, c_count;
    std::vector<int> CNF;

    readFile(input_file_path, CNF, v_count, c_count);

    int *CNF_array = new int[c_count*3];

    for (size_t i = 0; i < CNF.size(); i++) {
        CNF_array[i] = CNF[i];
   }

    printCudaInfo();

    SATCuda(CNF_array, v_count, c_count);

    delete [] CNF_array;

    return 0;
}
