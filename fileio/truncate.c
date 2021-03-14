/*
    Usage:
    truncate file length
*/

#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

void errorExit(char *format, const char *text) {
    printf(format, errno, text);
    perror("");
    exit(EXIT_FAILURE);
}

int
main(int argc, char *argv[])
{
    long length;

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        printf("%s file length\n", argv[0]);

    if((length = atol(argv[2])) == 0) {
        printf("'length' parameter is %s but must be an integer > 0", argv[2]);
        exit(EXIT_FAILURE);
    }
    
    if (truncate(argv[1], length) == -1)
        errorExit("error %d while truncating file %s", argv[1]);

    exit(EXIT_SUCCESS);
}
