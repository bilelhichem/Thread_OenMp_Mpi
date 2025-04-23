#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NB_THREAD 5

struct barrier_param {
    pthread_mutex_t *mutex;
    pthread_cond_t *cond;
    int counter_target;
    int *counter;
};

void *barrier(void *_arg)
{
    struct barrier_param *param = (struct barrier_param *)_arg;
    int rc;

    /* getting counter lock */
    rc = pthread_mutex_lock(param->mutex);
    if (rc) {
        fprintf(stderr, "Unable to lock: %d , %s .\n", rc, strerror(rc));
        pthread_exit(NULL);
    }

    /* adding me to counter */
    *param->counter = *param->counter + 1;
    /* checking if we are all here */
    if (*param->counter == param->counter_target) {
        /* all in : reset counter and signal other threads */
        printf("All here !\n");
        *param->counter = 0;
        rc = pthread_cond_broadcast(param->cond);
        if (rc) {
            fprintf(stderr, "Unable to broadcast: %d , %s .\n", rc,
                    strerror(rc));
            pthread_exit(NULL);
        }
    } else {
        /* waiting other threads : the last one will signal */
        printf("I'm waiting !\n");
        rc = pthread_cond_wait(param->cond, param->mutex);
        if (rc) {
            fprintf(stderr, "Unable to wait: %d , %s .\n", rc, strerror(rc));
            pthread_exit(NULL);
        }
    }

    /* release counter lock and exit */
    rc = pthread_mutex_unlock(param->mutex);
    if (rc) {
        fprintf(stderr, "Unable to unlock: %d , %s .\n", rc, strerror(rc));
        pthread_exit(NULL);
    }
    pthread_exit(NULL);
}

int main(int argc, char **argv)
{
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
    struct barrier_param thread_arg;
    pthread_t thread_id[NB_THREAD];
    int counter = 0;
    int rc;
    int i;

    /* init thread input parameter */
    thread_arg.mutex = &mutex;
    thread_arg.cond = &cond;
    thread_arg.counter_target = NB_THREAD;
    thread_arg.counter = &counter;

    /* creating threads */
    for (i = 0; i < NB_THREAD; i++) {
        rc = pthread_create(&thread_id[i], NULL, barrier, &thread_arg);
        if (rc) {
            fprintf(stderr, "Unable to create thread %d, rc: %d, %s .\n", i, rc,
                    strerror(rc));
            exit(EXIT_FAILURE);
        }
    }

    /* joining threads */
    for (i = 0; i < NB_THREAD; i++) {
        rc = pthread_join(thread_id[i], NULL);
        if (rc) {
            fprintf(stderr, "Unable to join thread %d, rc: %d, %s .\n", i, rc,
                    strerror(rc));
            exit(EXIT_FAILURE);
        }
    }

    exit(EXIT_SUCCESS);
}
