#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define BUF_SIZE 1024*10
#define PORT_NUM "6666"

void errorExit(char *format, const char *text) {
    printf(format, errno, text);
    perror("");
    exit(EXIT_FAILURE);
}

int open_file(const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        errorExit("error %d while opening file %s", filename);
    }
    return fd;
}

int establish_connection(const char *hostname) {
     // ----------- 1. Create socket object ------------------

    int conn_sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (conn_sd == -1) {
        fprintf(stderr, "Could not create socket: %s\n",
                strerror(errno));
        exit(1);
    }

    // ------------ 2. Connect to server-- ------------------

    // Find the IP address behind the hostname
    struct hostent* host_entry;
    host_entry = gethostbyname(hostname);
    if (!host_entry) {
        fprintf(stderr, "Could not resolve the host '%s'\n", hostname);
        exit(1);
    }

    // Prepare the address
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr = *((struct in_addr*)host_entry->h_addr);;
    addr.sin_port = htons(6666);

    int result = connect(conn_sd, (struct sockaddr*)&addr,
            sizeof(addr));
    if (result == -1) {
        close(conn_sd);
        fprintf(stderr, "Could no connect: %s\n", strerror(errno));
        exit(1);
    }
    return conn_sd;
}

int
main(int argc, char *argv[])
{
    ssize_t num_read;
    ssize_t tot_sent = 0;
    char buf[BUF_SIZE];

    if (argc < 4 || strcmp(argv[1], "--help") == 0) {
        printf("%s server-host file num-requests \n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sfd = establish_connection(argv[1]);
    //Multiple requests on same connection
    int num_req = atoi(argv[3]);
    for (size_t i = 0; i < num_req; i++) {
        
        int fd = open_file(argv[2]);

        while ((num_read = read(fd, buf, BUF_SIZE)) > 0) {
            if (write(sfd, buf, num_read) != num_read)
                errorExit("error %d while writing to socket %s\n", "");

            tot_sent += num_read;
            //printf("Total sent: %zd\n", tot_sent);
        }

        if (num_read == -1)
            errorExit("error %d while reading file %s\n", argv[2]);
        if (close(fd) == -1) perror("close file");   
          
    }
    if (close(sfd) == -1) perror("close socket"); 
    printf("Total sent: %zd\n", tot_sent);

    exit(EXIT_SUCCESS); /* Closes 'sfd' */
}
