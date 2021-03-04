/**
 * Copy the hexadecimal representation of the content of the file argv[1] into
 * the file argv[2]
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
    FILE *inputFd, *outputFd;
    size_t numRead;
    // 0 <= buf[i] <= 255
    unsigned char buf[BUF_SIZE];

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        printf("%s old-file new-file\n", argv[0]);

    /* Open input and output files */

    inputFd = fopen(argv[1], "rb");
    if (inputFd == NULL) {
        printf("error %d while opening file %s", errno, argv[1]);
        perror("");
        exit(EXIT_FAILURE);
    }

    outputFd = fopen(argv[2], "w");
    if (outputFd == NULL) {
        printf("error %d while opening file %s", errno, argv[2]);
        perror("");
        exit(EXIT_FAILURE);
    }

    /* Transfer data until we encounter end of input or an error */
    while ((numRead = fread(buf, 1, BUF_SIZE, inputFd)) > 0) {
        if (numRead < BUF_SIZE && ferror(inputFd)) {
            perror("read");
            printf("error %d while reading file %s", errno, argv[1]);
            exit(EXIT_FAILURE);
        }

        if (fwrite(buf, 1, numRead, outputFd) != numRead) {
            perror("write() returned error or partial write occurred");
            printf("error %d while writing file %s", errno, argv[1]);
            exit(EXIT_FAILURE);
        }
    }
    if (ferror(inputFd)) perror("read");

    if (fclose(inputFd) == EOF) perror("close input");
    if (fclose(outputFd) == EOF) perror("close output");

    exit(EXIT_SUCCESS);
}
