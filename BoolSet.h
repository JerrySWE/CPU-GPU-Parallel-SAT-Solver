#ifndef BOOLSET
#define BOOLSET

#include <cstddef>
#include <cstdlib>
#include <stdio.h>

/*
 * Each bit will correspond to a boolean assignment to a variable
 * 1-8 variables should have set size 1
 * 9-16 variables should have set size 2
 * 17-24 variables should have set size 3
 */

class BoolSet {
    public:
        size_t variable_count;
        size_t set_size;
        u_int8_t *bool_set;
    explicit BoolSet(size_t init_variable_count) {
        variable_count = init_variable_count;
        set_size = (variable_count - 1) / 8 + 1;
        bool_set = (u_int8_t *)calloc(set_size, sizeof(u_int8_t));
    }

    ~BoolSet() {
        free(bool_set);
    }

    bool getAssignment(size_t index) {
        size_t set_index = index / 8;
        size_t bit_index = index % 8;
        return (bool_set[set_index] >> bit_index) & 0x1;
    }

    bool incrementSet() {
        size_t curr_index = 0;
        int carry = 1;
        while (true) {
            if (curr_index >= set_size) {
                return false;
            }

            if (carry == 0) {
                return true;
            }

            if (bool_set[curr_index] == 255) {
                carry = 1;
                bool_set[curr_index] = 0;
                curr_index++;
            } else {
                bool_set[curr_index]++;
                return true;
            }
        }
    }
};

#endif