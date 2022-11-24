
// gcc unlink_file.c -o out/unlink_file

#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_SIZE 1024

int
main(int argc, char *argv[])
{
    char *filename = argv[1];      
    char buf[BUF_SIZE];  /* Random bytes to write to file */

    //num 1kB-blocks
    int numBlocks = 100000;

    /* O_EXCL to ensure we create a new file */
    int fd = open(filename, O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        printf("open error");
        exit(EXIT_FAILURE);
    }
    printf("file %s created, filename available in folder\n", filename);
    // sleep while we verify that filename is visible: ll -rti
    sleep(10);    

    // Remove filename
    if (unlink(filename) == -1) {
        printf("unlink error");
        exit(EXIT_FAILURE);
    }

    printf("file %s unlinked\n", filename);
    // sleep while we verify that filename is not visible: ll -rt
    // and yet the file is held open by the process: lsof -p $(ps -ef | awk '/out\/unlink_file/ {print $2}')
    // same inode lsof -p $(ps -ef | awk '/out\/unlink_file/ {print $2}') | awk '/ex/ {print "size:" $7; print "inode:" $8}'
    sleep(10);

    // the process can even write to the file
    for (int j = 0; j < numBlocks; j++) 
        if (write(fd, buf, BUF_SIZE) != BUF_SIZE) {
            printf("partial/failed write");
            exit(EXIT_FAILURE);
        }

    printf("after writing\n");
    // sleep again to verify the size of the file has increased
    sleep(10);

    // close desciptor and file is destroyed
    if (close(fd) == -1) {              
        printf("close error");
        exit(EXIT_FAILURE);
    }
    printf("file descriptor closed\n");
    // sleep again to verify that the file is no longer held by the process
    sleep(10);
    exit(EXIT_SUCCESS);
}
