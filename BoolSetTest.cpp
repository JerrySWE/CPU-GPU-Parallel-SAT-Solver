#include "BoolSet.h"
#include <stdio.h>

int main() {
    auto set1 = BoolSet(17);

    for (int i = 0; i < (1<<17); i++) {
        printf("**************************\n");
        for (int bool_i = 0; bool_i < 17; bool_i++) {
            auto assignment = set1.getAssignment(bool_i);
            printf("Variable: %d assigned as %d\n", bool_i, (int) assignment);
        }
        printf("**************************\n");
        set1.incrementSet();
    }    

    return 0;
}