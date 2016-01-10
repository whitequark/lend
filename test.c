#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#include "lend.h"

void *lend_malloc(size_t size);
void *lend_calloc(size_t numb, size_t size);
void lend_free(void *objp);

int main(int argc, char** argv) {
    if(argc != 4) {
        fprintf(stderr, "usage: %s [seed] [size] [inum]\n", argv[0]);
        return 1;
    }

    int seed = atoi(argv[1]),
        size = atoi(argv[2]),
        inum = atoi(argv[3]);

    printf("seed %d\n", seed);
    srand(seed);

    void *area = malloc(size);
    lend_give(area, size);

    int ptrc = 0;
    void **ptrv = calloc(size / sizeof(void*), sizeof(void *));

    for(int iter = 0; iter < inum; iter++) {
        if(getenv("SHOW")) lend_show();

        if(!ptrc || rand() < INT_MAX / 2) {
            if(getenv("SHOW")) printf("take\n");

            void **ptri;
            for(ptri = ptrv; *ptri; ptri++);
            *ptri = lend_calloc(1, rand() % (size / 2));
            if(*ptri && ptri - ptrv == ptrc)
                ptrc++;
        } else {
            if(getenv("SHOW")) printf("give\n");

            void **ptri = ptrv + (rand() % ptrc);
            lend_free(*ptri);
            *ptri = NULL;
        }
    }

    while(ptrc-- > 0) {
        lend_free(ptrv[ptrc]);
    }

    lend_free(lend_malloc(0));
    lend_show();

    return 0;
}
