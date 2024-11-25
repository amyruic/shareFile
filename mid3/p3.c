#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include<time.h>
#include<getopt.h>
#include<sys/mman.h>
#include<pthread.h>
#include <unistd.h>

#define MAXSIZE 1024*1024*1024
#define ITERATIONS 1000000
pthread_barrier_t wall;

int size=8192; // default size: control with -s command line argument
char shared[MAXSIZE];

void send(char* buf) {
    memcpy(shared,buf,size);
}

void recv(char* buf) {
#ifdef RECV
    memcpy(buf,shared,size);  
#endif

#ifdef REVRECV
    memcpy(shared,buf,size);  
#endif
}

void* sender(void* arg) {
    pthread_barrier_wait(&wall);
    uint64_t time=0;

    char buf[size];
    char buf2[size];
    memset(buf,0,size);
    for(int i=0;i<ITERATIONS;i++) {
        pthread_barrier_wait(&wall);

        struct timespec before,after;
        clock_gettime(CLOCK_MONOTONIC,&before);

        send(buf);

        clock_gettime(CLOCK_MONOTONIC,&after);
        time += ((after.tv_sec-before.tv_sec)*(uint64_t)1000000000+(after.tv_nsec-before.tv_nsec));
    }

    printf("Sender: %ld secs total, %lf GB/sec @ size %d\n",
    time/1000000000,1l*size*ITERATIONS*1000000000.0/time/1024/1024/1024,size);

}

void* receiver(void* arg) {
    pthread_barrier_wait(&wall);
    char buf[size];
    uint64_t time=0;

    for(int i=0;i<ITERATIONS;i++) {
        pthread_barrier_wait(&wall);

        struct timespec before,after;
        clock_gettime(CLOCK_MONOTONIC,&before);

        recv(buf);
        
        clock_gettime(CLOCK_MONOTONIC,&after);
        time += ((after.tv_sec-before.tv_sec)*(uint64_t)1000000000+(after.tv_nsec-before.tv_nsec));
    }

//    printf("Receiver: %ld secs total, %lf GB/sec @ size %d\n",
//    time/1000000000,1l*size*ITERATIONS*1000000000.0/time/1024/1024/1024,size);
}

int main(int argc, char** argv) {
	pthread_barrier_init(&wall,0,2);
    int opt;
    while ((opt = getopt(argc, argv, "s:")) != -1) {
        switch (opt) {
            case 's':
                size = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s -n <array_size>\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    struct timespec before,after;
    clock_gettime(CLOCK_MONOTONIC,&before);

    pthread_t one,two;
    pthread_create(&one,0,(void*)(void*)sender,0);
    pthread_create(&two,0,(void*)(void*)receiver,0);

    uint64_t retOne,retTwo;    
    if(pthread_join(one,(void**)&retOne) != 0) {
        perror("Error joining one\n");
        exit(1);
    }
    pthread_join(two,(void**)&retTwo);
   

}