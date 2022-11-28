#ifndef BIGINTREPR
#define BIGINTREPR

#include <cstddef>
#include <cstdlib>
#include <stdio.h>
#include <stdexcept>

/*
 * Each bit will correspond to a boolean assignment to a variable
 * 1-8 variables should have set size 1
 * 9-16 variables should have set size 2
 * 17-24 variables should have set size 3
 */

class BigIntRepr {
    public:
        size_t limit;
        size_t assignment;
        
    explicit BigIntRepr(size_t init_variable_count) {
        if (init_variable_count > 64) {
            throw std::invalid_argument("Big integer representation does not support more than 64 boolean variables!");
        }
        limit = 1 << init_variable_count;
        assignment = 0;
    }

    bool getAssignment(size_t index) {
        return ((assignment >> index) & 0x1) == 0x1;
    }

    bool incrementSet() {
        if (assignment >= limit) {
            return false;
        }
        assignment++;
        return true;
    }
};

#endif