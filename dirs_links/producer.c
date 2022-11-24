
/**
 * Create and write random data to a file passed as argument to the application.
 * 
 * Data is taken from a random 1Mb block of stack memory. The same chunk of data is written 10 times in
 * intervals of 6 seconds
 * 
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_SIZE 1024
#define NUM_REPETITIONS 5
#define WRITE_DELAY 3 //seconds

int
main(int argc, char *argv[])
{
    char *filename = argv[1];      
    char buf[BUF_SIZE];  // chunk of random bytes to write to file   
    
    // open file
    int fd = open(filename, O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open error");
        exit(EXIT_FAILURE);
    }  
    
    // write to file
    for (int j = 1; j <= NUM_REPETITIONS; j++) {
        if (write(fd, buf, BUF_SIZE) != BUF_SIZE) {
            perror("partial/failed write");
            exit(EXIT_FAILURE);
        }
        printf("blocks written: %d\n", j);
        sleep(WRITE_DELAY);
    }

    // close file desciptor
    if (close(fd) == -1) {              
        perror("close error");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
