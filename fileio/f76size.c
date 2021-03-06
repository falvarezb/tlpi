/*
    Return size of a file
*/

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

void errorExit(char *format, const char *text) {
    printf(format, errno, text);
    perror("");
    exit(EXIT_FAILURE);
}

int main(int argc, char const *argv[]) {

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        printf("%s file \n", argv[0]);
    
    int inputFd = open(argv[1], O_RDONLY);
    if (inputFd == -1) {
        errorExit("error %d while opening file %s", argv[1]);
    }

    off_t size;
    if ((size=lseek(inputFd, 0, SEEK_END)) == -1) {
        errorExit("error %d while lseek-ing file %s", argv[1]);
    }
    printf("%lld", size);
    exit(EXIT_SUCCESS);
}
