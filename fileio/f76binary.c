/**
 * Prints out the hexadecimal representation of the file argv[1]
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
    FILE *inputFile;
    size_t numRead;
    // 0 <= buf[i] <= 255
    unsigned char buf[BUF_SIZE];

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        printf("%s file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    inputFile = fopen(argv[1], "rb");
    if (inputFile == NULL) {
        printf("error %d while opening file %s", errno, argv[1]);
        perror("");
        exit(EXIT_FAILURE);
    }

    /* Transfer data until we encounter end of input or an error */
    while ((numRead = fread(buf, 1, BUF_SIZE, inputFile)) > 0) {
        if (numRead < BUF_SIZE && ferror(inputFile)) {
            perror("read error");
            printf("error %d while reading file '%s'", errno, argv[1]);
            exit(EXIT_FAILURE);
        }

        for (size_t i = 0; i < numRead; i++) {        
            if (printf("%.2x ", buf[i]) < 0 && ferror(stdout)) {
                perror("write error");
                printf("error %d while writing", errno);
                exit(EXIT_FAILURE);
            }
        }
    }

    if (ferror(inputFile)) {
        perror("read error");
        printf("error %d while reading file '%s'", errno, argv[1]);
        exit(EXIT_FAILURE);
    }

    if (fclose(inputFile) == EOF) perror("close input");    

    exit(EXIT_SUCCESS);
}
