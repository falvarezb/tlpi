#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <sys/resource.h>

static void /* Print 'msg' plus sysconf() value for 'name' */
sysconf_print(const char *msg, int name)
{   
    errno = 0;
    long lim = sysconf(name);
    if (lim != -1)
    { /* Call succeeded, limit determinate */
        printf("%s %ld\n", msg, lim);
    }
    else
    {
        if (errno == 0) /* Call succeeded, limit indeterminate */
            printf("%s (indeterminate)\n", msg);
        else
        { /* Call failed */
            printf("sysconf %s failed\n", msg);
            perror("");
        }
    }
}

static void             /* Print 'msg' plus value of fpathconf(fd, name) */
pathconf_print(const char *msg, char* fd, int name)
{    
    errno = 0;
    long lim = pathconf(fd, name);
    if (lim != -1) {        /* Call succeeded, limit determinate */
        printf("%s %ld\n", msg, lim);
    } else {
        if (errno == 0)     /* Call succeeded, limit indeterminate */
            printf("%s (indeterminate)\n", msg);
        else { /* Call failed */
            printf("pathconf %s failed\n", msg);
            perror("");
        }
    }
}

int main(int argc, char const *argv[])
{

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
    {
        printf("USAGE: %s num_files \n", argv[0]);
        exit(EXIT_FAILURE);
    }

    //set OPEN_MAX to 1000
    sysconf_print("OPEN_MAX:", _SC_OPEN_MAX);
    struct rlimit rl = {1000, RLIM_INFINITY};
    if(setrlimit(RLIMIT_NOFILE, &rl) == -1){
        perror("error setting open files limit");
    }
    sysconf_print("OPEN_MAX:", _SC_OPEN_MAX);

    size_t count = atoi(argv[1]);
    size_t i;    
    for (i = 0; i < count; i++)
    {
        char file_name[20];
        sprintf(file_name, "myfiles/f_%03zu", i);
        int inputFd = open(file_name, O_CREAT | O_RDONLY, S_IRUSR | S_IWUSR);
        if (inputFd == -1)
        {
            printf("error while opening file %s\n", file_name);
            perror("");
            break;
        }        
    }

    // print out pathnames of newly created files (using fd as parameter)
    // j<i+3 to account for stdin, stdout and stderror
    for (size_t j = 0; j < i + 3; j++)
    {
        char filePath[PATH_MAX];
        if (fcntl(j, F_GETPATH, filePath) == -1)
        {
            perror("fcntl error");
            break;
        }

        printf("fd=%zu, path=%s\n", j, filePath);
    }

    exit(EXIT_SUCCESS);
}