/**
 * Copy the hexadecimal representation of the content of the file argv[1] into the file argv[2]
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define BUF_SIZE 1024
#define BUF_EXPANSION_FACTOR 3

/*
    'num_copy' elements of orig_buffer are converted into hexadecimal and copied to dest_buffer
*/
void hex(unsigned char *dest_buffer, unsigned char *orig_buffer, int num_copy) {
    char buf_temp[BUF_EXPANSION_FACTOR + 1];
    for (size_t i = 0; i < num_copy; i++) {
        if (sprintf(buf_temp, "%.2x ", orig_buffer[i]) < 0) {
            printf("error %d while formatting output %c", errno,
                    orig_buffer[i]);
        }
        dest_buffer[i * 3] = buf_temp[0];      // first hexadecimal digit
        dest_buffer[i * 3 + 1] = buf_temp[1];  // second hexadecimal digit
        dest_buffer[i * 3 + 2] = buf_temp[2];  // whitespace
    }
}

int main(int argc, char *argv[]) {
    int inputFd, outputFd, openFlags;
    mode_t filePerms;
    ssize_t numRead;
    // 0 <= read_buf[i] <= 255
    unsigned char read_buf[BUF_SIZE];
    // each read_buf[i] is expanded into its hexadecimal representation + whitespace
    unsigned char write_buf[BUF_SIZE * BUF_EXPANSION_FACTOR];

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        printf("%s old-file new-file\n", argv[0]);

    /* Open input and output files */

    inputFd = open(argv[1], O_RDONLY);
    if (inputFd == -1) {
        printf("error %d while opening file %s", errno, argv[1]);
        perror("");
        exit(EXIT_FAILURE);
    }

    openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH |
                S_IWOTH; /* rw-rw-rw- */
    outputFd = open(argv[2], openFlags, filePerms);
    if (outputFd == -1) {
        printf("error %d while opening file %s", errno, argv[2]);
        perror("");
        exit(EXIT_FAILURE);
    }

    /* Transfer data until we encounter end of input or an error */
    while ((numRead = read(inputFd, read_buf, BUF_SIZE)) > 0) {
        hex(write_buf, read_buf, numRead);

        if (write(outputFd, write_buf, numRead * BUF_EXPANSION_FACTOR) != numRead * BUF_EXPANSION_FACTOR) {
            perror("write() returned error or partial write occurred");
            exit(EXIT_FAILURE);
        }
    }
    if (numRead == -1) perror("read");

    if (close(inputFd) == -1) perror("close input");
    if (close(outputFd) == -1) perror("close output");

    exit(EXIT_SUCCESS);
}
