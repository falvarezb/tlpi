/*
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

void errorExit(char *format, const char *text) {
    printf(format, errno, text);
    perror("");
    exit(EXIT_FAILURE);
}

int main(int argc, char const *argv[]) {
    off_t size, num_holes = 0, offset1 = 18, offset2, offset3;

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        printf("%s file \n", argv[0]);

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        errorExit("error %d while opening file %s", argv[1]);
    }

    if ((size = lseek(fd, 0, SEEK_END)) == -1) {
        errorExit("error %d while seeking in file %s", argv[1]);
    }

    while ((offset2 = lseek(fd, offset1, SEEK_DATA)) < size) {
        printf("offset = %ld\n", offset2);
        if (offset2 == -1) {
            errorExit("error %d while seeking in file %s", argv[1]);
        }

        if ((offset3 = lseek(fd, offset2, SEEK_DATA)) == -1) {
            printf("offset = %ld\n", offset2);
            errorExit("error %d while seeking in file %s", argv[1]);
        }
        num_holes += offset3 - offset2;
        offset1 = offset3;
    }

    if (close(fd) == -1) perror("close input");
    printf("num holes = %lld\n", num_holes);

    exit(EXIT_SUCCESS);
}
