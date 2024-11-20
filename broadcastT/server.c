#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
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
    int arg, port, server_socket, new_socket;
    struct sockaddr_in serverAddress;
    int epoll_fd = epoll_create1(0);
    char *pvalue = NULL;

    if (epoll_fd == -1){
        fprintf(stderr, "Could not create epoll file descriptor! \n");
        return 1;
    }
   

    // returns -1 when all args are parsed
    while ((arg = getopt(argc, argv, "p:")) != -1){
        switch (arg)  {
            case 'p':
                pvalue = optarg;
                break;
            case '?':
                if (optopt == 'p')
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                return 1;
            default:
                abort();
        }
    }

    // if user didn't provide a port, we tell'em to 
    if (pvalue == NULL){
        fprintf(stderr, "Usage: %s -p port\n", argv[0]);
        return 1;
    }
    // save port number
    port = atoi(pvalue);
    // create socket for server; address family, type, protocol
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket == -1){
        error("Could not create socket");
    }

    int opt =1;

    // Set SO_REUSEADDR to allow local address reuse
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // prep the sockaddr stucture
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("bind failed. Error");
        return 1;
    }

    listen(server_socket, 3);

    puts("Waiting for incoming connections..");
    arg = sizeof(struct sockaddr);

    while ((new_socket = accept(server_socket, (struct sockaddr*)&client, (socklen_t)&arg))){
        puts("Connection 取った!");
        struct epoll_event event;
        event.events = EPOLLIN;
	    event.data.fd = 0;
        if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, 0, ))




    }
    







    if(close(epoll_fd)){
        fprintf(stderr, "Failed to close the epoll");
            return 1;
    }

    return 0;
}

