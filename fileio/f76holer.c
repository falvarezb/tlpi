/*
    Counts number of holes (null bytes)
    Usage:
    f76holer file
*/

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define BUF_SIZE 1024

void errorExit(char *format, const char *text) {
    printf(format, errno, text);
    perror("");
    exit(EXIT_FAILURE);
}

int main(int argc, char const *argv[]) {
    ssize_t num_read, num_holes = 0;
    off_t file_size;
    char buf[BUF_SIZE];

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        printf("%s file \n", argv[0]);

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        errorExit("error %d while opening file %s", argv[1]);
    }

    if ((file_size = lseek(fd, 0, SEEK_END)) == -1) {
        errorExit("error %d while seeking in file %s", argv[1]);
    }

    if (lseek(fd, 0, SEEK_SET) == -1) {
        errorExit("error %d while seeking in file %s", argv[1]);
    }

    while ((num_read = read(fd, buf, BUF_SIZE)) > 0) {
        for (size_t i = 0; i < num_read; i++) {
            if (buf[i] == '\0') {
                num_holes++;
            }
        }
    }

    if (num_read == -1) errorExit("error %d while reading file %s", argv[1]);

    if (close(fd) == -1) perror("close input");
    printf("num holes = %ld, file size = %lld, %% holes = %.2f\n", num_holes, file_size, num_holes * 100 / (float) file_size);

    exit(EXIT_SUCCESS);
}
