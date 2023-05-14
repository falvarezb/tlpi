/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2020.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 29-1 */

/* simple_thread.c

   A simple POSIX threads example: create a thread, and then join with it.
*/
#include <pthread.h>
#include "tlpi_hdr.h"

static void *
threadFunc(void *arg)
{
    sleep(5);
    char *s = (char *) arg;

    printf("%s", s);

    return (void *) strlen(s);
}

static void * threadDoNothing(void *arg) {return NULL;}
static void * detachedThread(void *arg) {
    pthread_detach(pthread_self());
    return NULL;
}
static void * threadExit(void *arg) {
    printf("message from thread before exit\n");
    pthread_join(pthread_self(), NULL);
    //exit(EXIT_SUCCESS);
}

/**
 * main thread waits and joins child thread
 */
int case1() {
    pthread_t t1;
    void *res;    

    int s = pthread_create(&t1, NULL, threadFunc, "Hello world\n");
    if (s != 0)
        errExitEN(s, "pthread_create");

    printf("Message from main()\n");
    s = pthread_join(t1, &res);
    if (s != 0)
        errExitEN(s, "pthread_join");

    printf("Thread returned %ld\n", (long) res);

    exit(EXIT_SUCCESS);
}

/**
 * main thread ends its execution without waiting for child to finish and the entire process finishes
 */
int case2() {
    pthread_t t1;      

    int s = pthread_create(&t1, NULL, threadFunc, "Hello world\n");
    if (s != 0)
        errExitEN(s, "pthread_create");

    printf("Message from main()\n");
    return 0;
}

/**
 * main thread exits but child continues running
 */
int case3() {
    pthread_t t1;      

    int s = pthread_create(&t1, NULL, threadFunc, "Hello world\n");
    if (s != 0)
        errExitEN(s, "pthread_create");

    printf("Message from main()\n");
    pthread_exit(NULL);
}

/**
 * creation of zombie threads (child threads that are not joined by some other thread)
 */
void case4() {
    pthread_t t1;      
    int num_threads = 100000;
    for (int i = 0; i < num_threads; i++){
        int s = pthread_create(&t1, NULL, threadDoNothing, NULL);
        if (s != 0)
            errExitEN(s, "pthread_create");

        if(i%(num_threads/10) == 0) {
          printf("New thread created - %d\n", i);
        }
        //sleep(1);
    }
    printf("Message from main()\n");
}

/**
 * creation of detached threads (child threads that do not need to be joined)
 */
void case5() {
    pthread_t t1;      
    int num_threads = 100000;
    for (int i = 0; i < num_threads; i++){
        int s = pthread_create(&t1, NULL, detachedThread, NULL);
        if (s != 0)
            errExitEN(s, "pthread_create");

        if(i%(num_threads/10) == 0) {
          printf("New thread created - %d\n", i);
        }
        //sleep(1);
    }
    printf("Message from main()\n");
}

/**
 * an exit by any thread finishes all other threads and the entire process
 */
void case6() {
    pthread_t t1;          
    
    int s = pthread_create(&t1, NULL, threadExit, NULL);
    if (s != 0)
        errExitEN(s, "pthread_create");

    //sleep to give time for the child thread to exit
    sleep(5);    
    printf("This is never written\n");
}

/**
 * thread joining with itself
 */
void case7() {        
    
    pthread_join(pthread_self(), NULL);
    printf("This is never written\n");
}

int
main(int argc, char *argv[])
{
    case6();
    
}
