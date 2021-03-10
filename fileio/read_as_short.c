/*
    Read as shorts the bytes of the file generated with t_writev
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
    FILE *inputFd;
    size_t numRead;
    unsigned short buf[BUF_SIZE];

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        printf("%s file\n", argv[0]);

    inputFd = fopen(argv[1], "rb");
    if (inputFd == NULL) {
        printf("error %d while opening file %s", errno, argv[1]);
        perror("");
        exit(EXIT_FAILURE);
    }

    /* Transfer data until we encounter end of input or an error */
    while ((numRead = fread(buf, 2, BUF_SIZE, inputFd)) > 0) {
        if (numRead < BUF_SIZE && ferror(inputFd)) {
            perror("read");
            printf("error %d while reading file %s", errno, argv[1]);
            exit(EXIT_FAILURE);
        }

        for (size_t i = 0; i < numRead; i++) {
            if (printf("%d ", buf[i]) < 0 && ferror(stdout)) {
                perror("error or partial write occurred");
                printf("error %d while writing", errno);
                exit(EXIT_FAILURE);
            }
        }
    }
    if (ferror(inputFd)) perror("read");

    if (fclose(inputFd) == EOF) perror("close input");

    exit(EXIT_SUCCESS);
}
