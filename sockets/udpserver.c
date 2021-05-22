#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "tlpi_hdr.h"
#include "inet_sockets.h"

#define BUF_SIZE 1024
#define PORT_NUM "6666"

int
main(int argc, char *argv[])
{
    int sfd;
    ssize_t num_read, tot_received;                
    char buf[BUF_SIZE];

    /* Ignore the SIGPIPE signal, so that we find out about broken connection
       errors via a failure from write(). */

    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) errExit("signal");

    sfd = inetBind(PORT_NUM, SOCK_DGRAM, NULL);
    if (sfd == -1) errExit("error while opening socket");
    
    printf("listening for connections on port %s...\n", PORT_NUM);
            
    tot_received = 0;        
    
    while ((num_read = read(sfd, buf, BUF_SIZE)) > 0) {
        tot_received += num_read;
        printf("Total received: %zd\n", tot_received);
    }

    if (num_read == -1) errExit("error while reading client data"); 

    if (close(sfd) == -1)           /* Close connection */
        errMsg("close connection");
}
