#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>
#include <pthread.h>

void *thr_func()
{
    for (int i = 0; i < 5; i++)
    {
        printf("I am a Custom Thread Function Created By user.\n");
        sleep(1);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    printf("This program would create threads\n");
    pthread_t thread;
    if (pthread_create(&thread, NULL, &thr_func, NULL) == 0)
    {
        printf("Custom Thread Created Succesfully\n");
    }

    for (int i = 0; i < 5; i++)
    {
        printf("I am process thread created by the compiler by default.\n");
        sleep(1);
    }
    pthread_join(thread, NULL);
    return 0;
}

// printing will sometimes go out of order occasionally