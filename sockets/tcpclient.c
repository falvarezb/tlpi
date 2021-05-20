#include <netdb.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/socket.h>
#include "tlpi_hdr.h"
#include "inet_sockets.h"

#define BUF_SIZE 1024
#define PORT_NUM "6666"

int main(int argc, char *argv[]) {
    ssize_t num_read;
    ssize_t tot_sent = 0;    
    char buf[BUF_SIZE];   
    int sfd, fd; 

    if (argc < 4 || strcmp(argv[1], "--help") == 0) usageErr("%s server-host file num-requests\n", argv[0]);
    char *serverhost = argv[1];
    char *filename = argv[2];
    size_t num_req = atoi(argv[3]);

    if ((sfd = inetConnect(argv[1], PORT_NUM, SOCK_STREAM)) == -1) errExit("error while connecting to %s", serverhost);    

    //Multiple requests on same connection    
    for (size_t i = 0; i < num_req; i++) {        
        if((fd = open(filename, O_RDONLY)) == -1) errExit("error while opening file %s\n", filename);

        while ((num_read = read(fd, buf, BUF_SIZE)) > 0) {
            if (write(sfd, buf, num_read) != num_read) errExit("error while sending data");                    
            if (num_read == -1) errExit("error %d while reading file %s\n", filename);
            tot_sent += num_read;
        }
        
        if (close(fd) == -1) errMsg("close input");        
    }
    printf("Total sent: %zd\n", tot_sent);

    exit(EXIT_SUCCESS); /* Closes 'sfd' */
}
