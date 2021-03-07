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

    if (argc < 3 || strcmp(argv[1], "--help") == 0)
        printf("%s file text \n", argv[0]);
    
    int fd = open(argv[1], O_WRONLY | O_CREAT | O_APPEND);
    if (fd == -1) {
        errorExit("error %d while opening file %s", argv[1]);
    }

    //fruitless re-positioning, O_APPEND takes precedence
    off_t size;
    if ((size=lseek(fd, 0, SEEK_SET)) == -1) {
        errorExit("error %d while seeking in file %s", argv[1]);
    }

    if (write(fd, argv[2], strlen(argv[2])) != strlen(argv[2])) {
        errorExit("error %d while writing file %s", argv[1]);        
    }
    
    exit(EXIT_SUCCESS);
}
