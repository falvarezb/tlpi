/**
 * Copy the hexadecimal representation of the content of the file argv[1] into
 * the file argv[2]
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 1024
#define BUF_EXPANSION_FACTOR 3

/*
    'num_copy' elements of orig_buffer are converted into hexadecimal and copied
   to dest_buffer
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
    FILE *inputFd, *outputFd;
    size_t numRead;
    // 0 <= read_buf[i] <= 255
    unsigned char read_buf[BUF_SIZE];
    // each read_buf[i] is expanded into its hexadecimal representation +
    // whitespace
    unsigned char write_buf[BUF_SIZE * BUF_EXPANSION_FACTOR];

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        printf("%s old-file new-file\n", argv[0]);

    /* Open input and output files */

    inputFd = fopen(argv[1], "r");
    if (inputFd == NULL) {
        printf("error %d while opening file %s", errno, argv[1]);
        perror("");
        exit(EXIT_FAILURE);
    }

    outputFd = fopen(argv[2], "wb");
    if (outputFd == NULL) {
        printf("error %d while opening file %s", errno, argv[2]);
        perror("");
        exit(EXIT_FAILURE);
    }

    /* Transfer data until we encounter end of input or an error */
    while ((numRead = fread(read_buf, 1, BUF_SIZE, inputFd)) > 0) {
        if (numRead < BUF_SIZE && ferror(inputFd)) {
            perror("read");
            printf("error %d while reading file %s", errno, argv[1]);
            exit(EXIT_FAILURE);
        }

        hex(write_buf, read_buf, numRead);

        if (fwrite(write_buf, 1, numRead * BUF_EXPANSION_FACTOR, outputFd) != numRead * BUF_EXPANSION_FACTOR) {
            perror("write() returned error or partial write occurred");
            exit(EXIT_FAILURE);
        }
    }
    if (ferror(inputFd)) perror("read");

    if (fclose(inputFd) == EOF) perror("close input");
    if (fclose(outputFd) == EOF) perror("close output");

    exit(EXIT_SUCCESS);
}
