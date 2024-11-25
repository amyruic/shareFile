#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include<time.h>
#include<getopt.h>
#include<sys/mman.h>

char *array;
uint64_t dep_read() {
    uint64_t index=0;
    uint64_t sum=0;
    for(uint64_t i=0;i<(uint64_t)100;i++) {
        uint64_t arrayval=array[index];
        sum+=arrayval;
        index=(index+arrayval+STRIDE)%ARRSIZE;
    }
    return sum;
}

int main(int argc, char** argv) {
    array=mmap((void*)0,ARRSIZE,PROT_WRITE|PROT_READ,MAP_SHARED|MAP_ANONYMOUS,0,0);
    struct timespec before,after;
    clock_gettime(CLOCK_THREAD_CPUTIME_ID,&before);
    uint64_t ret=dep_read();
    clock_gettime(CLOCK_THREAD_CPUTIME_ID,&after);
    uint64_t time = ((after.tv_sec-before.tv_sec)*(uint64_t)1000000000+(after.tv_nsec-before.tv_nsec));
    printf("%d elements, %lf cycles/element. Sum: %ld.\n",ARRSIZE,2*time/(double)100,ret);

}
