/**
 * Implementation of call system 'dup' in terms of 'fcntl'
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void errorExit(char *format, int text) {
    printf(format, errno, text);
    perror("");
    exit(EXIT_FAILURE);
}

int atolzero(const char *c) {
    int result;
    if (*c == '0') {
        return 0;
    } else if ((result = atol(c)) == 0) {
        printf("cursor_position parameter is %s but must be an integer >= 0",
               c);
        exit(EXIT_FAILURE);
    }
    return result;
}

int mydup(int oldfd) {
    int newfd;
    if ((newfd = fcntl(oldfd, F_DUPFD, 0)) == -1) {
        errorExit("error %d while duplicating file descriptor %d", oldfd);
    }
    return newfd;
}

int mydup2(int oldfd, int newfd) {
    if(fcntl(oldfd, F_GETFL) == -1){
        errorExit("error %d while duplicating file descriptor %d", oldfd);
    }

    if(oldfd == newfd) {
        return newfd;
    }

    close(newfd);
    if (fcntl(oldfd, F_DUPFD, newfd) == -1) {
        errorExit("error %d while duplicating file descriptor %d", oldfd);
    }
    return newfd;
}

int main(int argc, char const *argv[]) {
    int oldfd, newfd;

    if (argc == 2)
    {
        oldfd = atolzero(argv[1]);
        newfd = mydup(oldfd);
    }
    else if(argc == 3){
        oldfd = atolzero(argv[1]);
        newfd = atolzero(argv[2]);
        newfd = mydup2(oldfd, newfd);
    }
    else {
        printf("%s oldfd [newfd] \n", argv[0]);
        exit(EXIT_FAILURE);
    }

    printf("newfd: %d\n", newfd);
    exit(EXIT_SUCCESS);
}
