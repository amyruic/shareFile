#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <string.h>
#include <stdatomic.h>


#define NUM_THREADS 16
#define NUM_ITERATIONS 1000000

#ifdef USE_MUTEX
typedef pthread_mutex_t lock_t;
#define lock_name "mutex"
#define lock_acquire pthread_mutex_lock
#define lock_release pthread_mutex_unlock
#define lock_init pthread_mutex_init
#else
#define lock_name "spinlock"
typedef pthread_spinlock_t lock_t;
#define lock_acquire pthread_spin_lock
#define lock_release pthread_spin_unlock
#define lock_init pthread_spin_init
#endif

#ifndef DS
#define DS 0
#endif

struct item {
    lock_t lock;
    int count;
    char data[DS];
};

struct item items[NUM_THREADS];

void *threadFunc(void *threadid) {
    long tid = (long)threadid;
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        lock_acquire(&items[tid].lock);
        items[tid].count++;
        lock_release(&items[tid].lock);
    }
}

int main() {
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (t = 0; t < NUM_THREADS; t++) {
        lock_init(&items[t].lock,0);
	//printf("%d: %p\n",t,&items[t]);
        rc = pthread_create(&threads[t], NULL, threadFunc, (void *)t);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    for (t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    long elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);

    printf("With DS=%d size is %ld and %s, counting took %lf ns per item.\n",DS,sizeof(struct item),lock_name,(double)elapsed_ns/(double)NUM_ITERATIONS);
    return 0;
}

