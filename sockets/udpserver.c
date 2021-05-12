#define _BSD_SOURCE             /* To get definitions of NI_MAXHOST and
                                   NI_MAXSERV from <netdb.h> */
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>


#define BACKLOG 50
#define BUF_SIZE 1024
#define PORT_NUM "6666"

void errorExit(char *format, const char *text) {
    printf(format, errno, text);
    perror("");
    exit(EXIT_FAILURE);
}

int open_listener_socket() {
    // ----------- 1. Create socket object ------------------
    int server_sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_sd == -1) {
        fprintf(stderr, "Could not create socket: %s\n",
                strerror(errno));
        exit(1);
    }

    // ----------- 2. Bind the socket file ------------------

    // Prepare the address
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(6666);

    int result = bind(server_sd,
            (struct sockaddr*)&addr, sizeof(addr));
    if (result == -1) {
        close(server_sd);
        fprintf(stderr, "Could not bind the address: %s\n",
                strerror(errno));
        exit(1);
    }

    return server_sd;
}

int
main(int argc, char *argv[])
{
    ssize_t num_read;
    ssize_t tot_received = 0;
    struct sockaddr_storage claddr;
    int lfd, cfd, reqLen;
    socklen_t addrlen;
#define ADDRSTRLEN (NI_MAXHOST + NI_MAXSERV + 10)
    char addrStr[ADDRSTRLEN];
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];
    char buf[BUF_SIZE];


    /* Ignore the SIGPIPE signal, so that we find out about broken connection
       errors via a failure from write(). */

    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)    perror("signal");

    cfd = open_listener_socket();
    printf("listening for connections on port %s...\n", PORT_NUM);    
                
    //sleep(10);
    while ((num_read = read(cfd, buf, BUF_SIZE)) > 0) {
        tot_received += num_read;
        printf("Total received: %zd\n", tot_received); 
    }

    if (num_read == -1) errorExit("error %d while reading socket %s\n", "");
    
    close(cfd);
    printf("end\n");

}
