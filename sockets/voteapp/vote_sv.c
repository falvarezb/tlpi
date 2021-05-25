#include <string.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdbool.h>
#include "tlpi_hdr.h"
#include "inet_sockets.h"
#include "vote_protocol.h"
#include "vote_encoding.h"
#include "framer.h"

#define BACKLOG 50
#define ADDRSTRLEN (NI_MAXHOST + NI_MAXSERV + 10)

int main(int argc, char *argv[]) {  

    int counts[MAX_CANDIDATE] = {0};

    if (argc < 2 || strcmp(argv[1], "--help") == 0) usageErr("%s server-port\n", argv[0]);
    char *serverport = argv[1];

    int tcpfd;
    // ssize_t num_write, num_read;                
    // char buf[BUF_SIZE];    

    struct client_data {
        int cfd;
        struct sockaddr_storage claddr;
        socklen_t addrlen;
        char addrStr[ADDRSTRLEN];
    } cldata;

    if ((tcpfd = inetListen(serverport, BACKLOG, NULL)) == -1) errExit("error while opening tcp socket");
    printf("listening for tcp connections on port %s...\n", serverport);

    while(1) { //process clients iteratively

        cldata.addrlen = sizeof(struct sockaddr_storage);                
        if ((cldata.cfd = accept(tcpfd, (struct sockaddr *) &cldata.claddr, &cldata.addrlen)) == -1) {
            errMsg("error while connecting to client");
            continue;
        }        
        printf("Connection from %s\n", inetAddressStr((struct sockaddr *) &cldata.claddr, cldata.addrlen, cldata.addrStr, ADDRSTRLEN));  

        FILE *channel = fdopen(cldata.cfd,"r+"); // Wrap for stream I/O
        if(channel == NULL)
            errExit("error while opening I/O channel\n");


        // Receive messages until connection closes
        size_t msg_size;
        uint8_t inbuf[MAX_WIRE_SIZE];
        vote_info v;

        while ((msg_size = get_next_msg(channel, inbuf, MAX_WIRE_SIZE)) > 0) {
            memset(&v, 0, sizeof(v)); // Clear vote information
            printf("Received message (%d bytes)\n", msg_size);

            if (decode(inbuf, msg_size, &v)) {
                if (!v.isResponse) { // Ignore non-requests
                    v.isResponse = true;
                    if (v.candidate >= 0 && v.candidate <= MAX_CANDIDATE) {
                        if (!v.isInquiry)
                            counts[v.candidate] += 1;
                        v.count = counts[v.candidate];
                    } else {
                        // Ignore invalid candidates
                    }

                    uint8_t outbuf[MAX_WIRE_SIZE];
                    msg_size = encode(&v, outbuf, MAX_WIRE_SIZE);
                    if(put_msg(outbuf, msg_size, channel) < 0) {
                        errMsg("error framing/outputting message, closing connection");
                        break;
                    } else {
                        printf("Processed %s for candidate %d; current count is %llu.\n", (v.isInquiry ? "inquiry" : "vote"), v.candidate, v.count);
                    }
                    fflush(channel);
                }
            } else {
                errMsg("parsing error, closing connection");
                break;
            }
        }
        printf("client finished, closing connection");
        fclose(channel);
    }
}