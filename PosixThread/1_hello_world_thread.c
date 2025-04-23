#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *hello_world(void *nothing)
{
    printf("Hello world from thread!\n");
    pthread_exit(NULL);
}

int main(int argc, char **argv)
{
    pthread_t thread_id;
    int rc;

    /* create thread */
    rc = pthread_create(&thread_id, NULL, hello_world, NULL);
    if (rc) {
        fprintf(stderr, "Error on creating thread: %d, %s.\n",
                rc, strerror(rc));
        exit(EXIT_FAILURE);
    }

    /* join thread */
    rc = pthread_join(thread_id, NULL);
    if (rc) {
        fprintf(stderr, "Error on joining thread: %d, %s.\n", rc, strerror(rc));
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
