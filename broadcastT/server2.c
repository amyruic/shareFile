#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define MAX_CLIENTS 20

int client_soc[MAX_CLIENTS];

pthread_mutex_t client_mutex[MAX_CLIENTS] = {PTHREAD_MUTEX}
