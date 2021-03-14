/*
    Usage:
    write file offset text
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
    off_t offset;

    if (argc != 4 || strcmp(argv[1], "--help") == 0)
        printf("%s file offset text \n", argv[0]);

    if (*argv[2] == '0') {
        offset = 0;
    } else if ((offset = atol(argv[2])) == 0) {
        printf("cursor_position parameter is %s but must be an integer >= 0",
               argv[2]);
        exit(EXIT_FAILURE);
    }

    int fd = open(argv[1], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        errorExit("error %d while opening file %s", argv[1]);
    }

    if (lseek(fd, offset, SEEK_SET) == -1) {
        errorExit("error %d while seeking in file %s", argv[1]);
    }

    if (write(fd, argv[3], strlen(argv[3])) != strlen(argv[3])) {
        errorExit("error %d while writing file %s", argv[1]);
    }

    if (close(fd) == -1) perror("close input");

    exit(EXIT_SUCCESS);
}
