#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NUM 508
#define MAX_ITEM 40
#define NUM_ITERATIONS 1000000

typedef struct {  
    int len;
    char num[MAX_NUM];
} Item;

Item items[MAX_ITEM];

int count(char c) {
    int count=0;
    int offset=0;
    while(offset<MAX_ITEM) {
        offset+=items[offset].num[0];
        count++;
    }
    return count;
}

int main() {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    memset(items,1,sizeof(items));

    int sum=0;
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        sum+=count('e');
        asm("":::"memory");
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    long elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);

    printf("Counting took %lf ns per item.\n",(double)elapsed_ns/(double)NUM_ITERATIONS/(double)MAX_ITEM);
    return sum;
}
