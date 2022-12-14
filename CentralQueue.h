#ifndef CQ
#define CQ

#include <cstddef>
#include <cstdlib>
#include <stdio.h>
#include <stdexcept>
#include <mutex>
#include <functional>

/*
 * Each bit will correspond to a boolean assignment to a variable
 * since size_t is only 64 bits, this boolean assignment will contain only 
 */

class CentralQueue {
    public:
        std::mutex lock;
        size_t limit;
        size_t granularity;
        size_t curr_assign;
        
    explicit CentralQueue(size_t v_count, size_t init_granularity) {
        if (v_count > 64) {
            throw std::invalid_argument("Central queue does not support more than 64 boolean variables!");
        }
        size_t base = 1;
        limit = base << v_count;
        curr_assign = 0;
        granularity = init_granularity;
    }

    bool FetchTask(size_t &start, size_t &end) {
        if (curr_assign >= limit) {
            return false;
        }

        lock.lock();
        start = curr_assign;
        end = std::min(curr_assign+granularity, limit);
        curr_assign += granularity;
        lock.unlock();
        return true;
    }
};

#endif