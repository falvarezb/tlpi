#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "tlpi_hdr.h"
#include "inet_sockets.h"

#define BACKLOG 50
#define BUF_SIZE 1024
#define PORT_NUM "6666"
#define ADDRSTRLEN (NI_MAXHOST + NI_MAXSERV + 10)

int
main(int argc, char *argv[])
{
    int lfd;
    ssize_t num_read, tot_received;                
    char buf[BUF_SIZE];

    struct client_data {
        int cfd;
        struct sockaddr_storage claddr;
        socklen_t addrlen;
        char addrStr[ADDRSTRLEN];
    } cldata;


    /* Ignore the SIGPIPE signal, so that we find out about broken connection
       errors via a failure from write(). */

    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) errExit("signal");

    lfd = inetListen(PORT_NUM, BACKLOG, NULL);
    if (lfd == -1) errExit("error while opening passive socket");
    
    printf("listening for connections on port %s...\n", PORT_NUM);

    while(1) {                  /* Handle clients iteratively */

        tot_received = 0;        

        cldata.addrlen = sizeof(struct sockaddr_storage);
        cldata.cfd = accept(lfd, (struct sockaddr *) &cldata.claddr, &cldata.addrlen);
        if (cldata.cfd == -1) {
            errMsg("error while connecting to client");
            continue;
        }        
        printf("Connection from %s\n", inetAddressStr((struct sockaddr *) &cldata.claddr, cldata.addrlen, cldata.addrStr, ADDRSTRLEN));
        
        
        while ((num_read = read(cldata.cfd, buf, BUF_SIZE)) > 0) {
            tot_received += num_read;
        }

        if (num_read == -1) errExit("error while reading client data");

        printf("Total received: %zd\n", tot_received); 

        if (close(cldata.cfd) == -1)           /* Close connection */
            errMsg("close connection");
    }
}
