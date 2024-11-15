#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/epoll.h>

#define MAX_CLIENTS 20

int client_soc[MAX_CLIENTS];

int num_clients = 0;

void error(const char *errmsg){
    perror(errmsg);
    exit(1);
}

int main () {
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1){
        fprintf(stderr, "Could not create epoll file descriptor! \n");
        return 1;
    }
    if(close(epoll_fd)){
        fprintf(stderr, "Failed to close the epoll");
            return 1;
    }
}
