#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
#include "tlpi_hdr.h"
#include "inet_sockets.h"
#include "byte_counter.h"

#define BACKLOG 50
#define ADDRSTRLEN (NI_MAXHOST + NI_MAXSERV + 10)

ssize_t tot_received;

void* tcp_handler(void* arg) {
    
    int tcpfd;
    ssize_t num_write, num_read;                
    char buf[BUF_SIZE];    

    struct client_data {
        int cfd;
        struct sockaddr_storage claddr;
        socklen_t addrlen;
        char addrStr[ADDRSTRLEN];
    } cldata;

    if ((tcpfd = inetListen(TCP_PORT_NUM, BACKLOG, NULL)) == -1) errExit("error while opening tcp socket");
    printf("listening for tcp connections on port %s...\n", TCP_PORT_NUM);

    while(1) {               

        cldata.addrlen = sizeof(struct sockaddr_storage);                
        if ((cldata.cfd = accept(tcpfd, (struct sockaddr *) &cldata.claddr, &cldata.addrlen)) == -1) {
            errMsg("error while connecting to client");
            continue;
        }        
        printf("Connection from %s\n", inetAddressStr((struct sockaddr *) &cldata.claddr, cldata.addrlen, cldata.addrStr, ADDRSTRLEN));  

        //read instructions  
        while ((num_read = read(cldata.cfd, buf, BUF_SIZE)) > 0) {               
            char *instruction = strtok(buf, INSTRUCTION_DELIMITER);            
            while (instruction != NULL) {                
                if (strcmp(instruction, RESET_COUNTER) == 0) {                    
                    tot_received = 0;
                    if (write(cldata.cfd, RESET_COUNTER_ACK, strlen(RESET_COUNTER_ACK)) != strlen(RESET_COUNTER_ACK)) errMsg("write() returned error or partial write occurred\n");
                } else if (strcmp(instruction, RETURN_COUNTER) == 0) {                    
                    num_write = snprintf(buf, BUF_SIZE, "%zd", tot_received);
                    if(num_write > 0 && num_write < BUF_SIZE) {                        
                        if (write(cldata.cfd, buf, num_write) != num_write) errMsg("write() returned error or partial write occurred\n");
                    } 
                    else if(num_write < 0) errMsg("encoding error when marshalling 'tot_received'\n");                    
                    else errMsg("buffer size too small %d\n", BUF_SIZE);
                    
                } else {                    
                    //ignore unknown instruction
                    printf("unknown instruction %s\n", instruction);
                }  
                instruction = strtok(NULL, INSTRUCTION_DELIMITER);
            }  
            memset(buf, 0, BUF_SIZE);                 
        }

        if (num_read == -1) {
            errMsg("error while reading client data"); 
            if (close(cldata.cfd) == -1) errMsg("close connection");
            continue;
        }                             
        if (close(cldata.cfd) == -1) errMsg("close connection");        
    }

    if (close(tcpfd) == -1) errMsg("close connection");
    return NULL;
}

void* udp_handler(void* arg) {
    int udpfd;
    ssize_t num_read;                
    char buf[BUF_SIZE];

    if ((udpfd = inetBind(UDP_PORT_NUM, SOCK_DGRAM, NULL)) == -1) errExit("error while opening udp socket");
    printf("listening for udp connections on port %s...\n", UDP_PORT_NUM);
                        
    while ((num_read = read(udpfd, buf, BUF_SIZE)) > 0) {
        tot_received += num_read;
        printf("Total received: %zd\n", tot_received);
    }

    if (num_read == -1) errExit("error while reading client data"); 
    if (close(udpfd) == -1) errMsg("close connection");

    return NULL;
}

int
main(int argc, char *argv[])
{
    pthread_t tcp_thread, udp_thread;

    /* Ignore the SIGPIPE signal, so that we find out about broken connection
       errors via a failure from write(). */
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) errExit("signal");

    if(pthread_create(&tcp_thread, NULL, tcp_handler, NULL) != 0) errExit("error while creating tcp thread");
    if(pthread_create(&udp_thread, NULL, udp_handler, NULL) != 0) errExit("error while creating udp thread");

    if(pthread_join(tcp_thread, NULL) != 0) errMsg("error while waiting for tcp thread");
    if(pthread_join(udp_thread, NULL) != 0) errMsg("error while waiting for udp thread");   

    exit(EXIT_SUCCESS); 
}
