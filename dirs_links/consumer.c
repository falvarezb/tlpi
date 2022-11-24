
/**
 * Read data from the file file created by the consumer and write it to standard output.
 * 
 * File is unlinked soon after being opened so that no other consumer can read it
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_SIZE 1024
#define READ_DELAY 3 //seconds

int
main(int argc, char *argv[])
{
    char *filename = argv[1];      
    char buf[BUF_SIZE];

    // open read-only file
    int fd = open(filename, O_RDONLY, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open error");
        exit(EXIT_FAILURE);
    }    

    // remove filename so that no other process can read it
    if (unlink(filename) == -1) {
        perror("unlink error");
        exit(EXIT_FAILURE);
    }    
    
    // data is read in chunks at intervals of 6 seconds
    ssize_t numRead;
    while ((numRead = read(fd, buf, BUF_SIZE))) {
        for (size_t i = 0; i < numRead; i++) {
            // printable chars
            if (buf[i] >= 33 && buf[i] < 127) {                
                printf("%c", buf[i]);
            }                     
        }              
        printf("\n"); 
        sleep(READ_DELAY);   
    }
    if (numRead == -1) {
        perror("read error");
        exit(EXIT_FAILURE);
    }

    // close desciptor and file is destroyed
    if (close(fd) == -1) {              
        perror("close error");
        exit(EXIT_FAILURE);
    }
    printf("file descriptor closed\n");
    sleep(5);
    exit(EXIT_SUCCESS);
}
