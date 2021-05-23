#include <netdb.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/socket.h>
#include "tlpi_hdr.h"
#include "inet_sockets.h"
#include "byte_counter.h"

#define BUF_SIZE 1024
#define UDP_PORT_NUM "6666"
#define TCP_PORT_NUM "6666"

size_t send_file(const char* filename, int sfd) {
    int fd;
    ssize_t num_read, tot_sent = 0;
    char buf[BUF_SIZE];  

    if((fd = open(filename, O_RDONLY)) == -1) errExit("error while opening file %s\n", filename);

    while ((num_read = read(fd, buf, BUF_SIZE)) > 0) {
        if (num_read == -1) errExit("error while reading file %s\n", filename);
        if (write(sfd, buf, num_read) != num_read) errExit("error while sending data\n");                                
        tot_sent += num_read;
    }
    
    if (close(fd) == -1) errMsg("close input\n");    
    return tot_sent;    
}

int main(int argc, char *argv[]) {    
    ssize_t num_read, tot_sent = 0;    
    char buf[BUF_SIZE];   
    int udpfd, tcpfd, fd; 
    char *msg;

    if (argc < 4 || strcmp(argv[1], "--help") == 0) usageErr("%s server-host file num-requests\n", argv[0]);
    char *serverhost = argv[1];
    char *filename = argv[2];
    size_t num_req = atoi(argv[3]);

    //initiating dialogue with TCP endpoint
    if ((tcpfd = inetConnect(argv[1], TCP_PORT_NUM, SOCK_STREAM)) == -1) errExit("error while connecting to %s:%s", serverhost, TCP_PORT_NUM);
    msg = RESET_COUNTER INSTRUCTION_DELIMITER;
    if (write(tcpfd, msg, strlen(msg)) != strlen(msg)) errExit("error while sending data\n");
    if ((num_read = read(tcpfd, buf, BUF_SIZE-1)) < 0) errExit("error while reading\n");
    buf[num_read] = '\0';
    if (strcmp(buf, RESET_COUNTER_ACK) != 0) errExit("unexpected message\n", buf);

    //initiating dialogue with UDP endpoint
    if ((udpfd = inetConnect(argv[1], UDP_PORT_NUM, SOCK_DGRAM)) == -1) errExit("error while connecting to %s:%s", serverhost, UDP_PORT_NUM);      
        
    for (size_t i = 0; i < num_req; i++) {        
        tot_sent += send_file(filename, udpfd);      
    }
    printf("Total sent: %zd\n", tot_sent);
    if (close(udpfd) == -1) errMsg("close udp connection");
    //end UDP

    msg = RETURN_COUNTER INSTRUCTION_DELIMITER;
    if (write(tcpfd, msg, strlen(msg)) != strlen(msg)) errExit("error while sending data\n");    
    if ((num_read = read(tcpfd, buf, BUF_SIZE-1)) <= 0) errExit("error while reading num bytes\n");  
    buf[num_read] = '\0';   
    printf("Total received: %s\n", buf);
    printf("received/sent: %f\n", strtof(buf, NULL) / tot_sent);
    if (close(tcpfd) == -1) errMsg("close tcp connection");
    //end TCP

    exit(EXIT_SUCCESS);
}
