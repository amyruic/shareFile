#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MB 1024*1024
#ifndef SIZE
#define SIZE 16
#endif

void* ptrs[100];

void malloc_many(int size) {
    for (int i = 0; i < 100; i++) {
        ptrs[i] = malloc(size);
    }
}
void free_many() {
    for (int i = 0; i < 100; i++) {
        free(ptrs[i]);
    }
}

int main(int argc, char** argv) {
    malloc_many(SIZE*MB);
    free_many();

    malloc_many(SIZE*MB);
    #ifdef MALLOC_TURBO
    int *chunk = malloc(100);
    #endif
    free_many();

    struct timespec start, end;
    long total_time_ns = 0;

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int r=0;r<1000;r++) {
        malloc_many(SIZE*MB);
        free_many();
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    total_time_ns = (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);
    printf("Average time per chunk: %ld ns\n", total_time_ns / 1000/100);
}