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

int main(int argc, char *argv[]) {  

    int sfd;  

    if (argc < 3 || strcmp(argv[1], "--help") == 0) usageErr("%s server-host server-port candidate [inquiry]\n", argv[0]);
    char *serverhost = argv[1];
    char *serverport = argv[2];
    int candidate = atoi(argv[3]);
    bool inq = argc > 4 && strcmp(argv[4], "I") == 0;

    if ((sfd = inetConnect(argv[1], serverport, SOCK_STREAM)) == -1) 
        errExit("error while connecting to %s:%s", serverhost, serverport);

    FILE *stream = fdopen(sfd,"r+"); // Wrap for stream I/O
    if(stream == NULL)
        errExit("error while opening I/O stream\n");
    
    // Set up info for a request
    vote_info vi;
    memset(&vi, 0, sizeof(vi));

    vi.isInquiry = inq;
    vi.candidate = candidate;

    //encode for transmission
    uint8_t outbuf[MAX_WIRE_SIZE];
    size_t req_size = encode(&vi, outbuf, MAX_WIRE_SIZE);

    // Print info
    printf("Sending %zu-byte %s for candidate %d...\n", req_size, (inq ? "inquiry" : "vote"), candidate);

    // Frame and send
    if (put_msg(outbuf, req_size, stream) < 0)
        errExit("error while putting message\n");

    // Receive and print response
    uint8_t inbuf[MAX_WIRE_SIZE];
    size_t resp_size = get_next_msg(stream, inbuf, MAX_WIRE_SIZE);

    if (decode(inbuf, resp_size, &vi)) {
        printf("Received:\n");
        if (vi.isResponse) {
            printf("  Response to ");
            if(vi.isInquiry)
                printf("inquiry ");
            else
                printf("vote ");
            printf("for candidate %d\n", vi.candidate);
            if (vi.isResponse)
                printf("  count = %llu\n", vi.count);
        }
    }
    fclose(stream);

    exit(EXIT_SUCCESS);
}