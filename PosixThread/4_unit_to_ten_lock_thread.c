#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NB_WAITED_SECOND 3
#define NB_THREAD 4

struct shared_abacus {
    pthread_mutex_t *unit_lock;
    unsigned int *nb_unit;
};

void *counter_to_ten(void *_abacus) {
    struct shared_abacus *abacus = (struct shared_abacus *)_abacus;
    unsigned int random_seed;
    int rc;

    /* init random */
    random_seed = time(NULL);

    do {
        /* wait a bit */
        sleep(rand_r(&random_seed) % MAX_NB_WAITED_SECOND);

        /* take unit lock */
        rc = pthread_mutex_lock(abacus->unit_lock);
        if (rc) {
            fprintf(stderr, "Thread unable to take unit_lock : %d , %s .\n",
                    rc, strerror(rc));
            pthread_exit(NULL);
        }

        /* do we have already reach hundred ? */
        if (*abacus->nb_unit == 10) {
            rc = pthread_mutex_unlock(abacus->unit_lock);
            if (rc) {
                fprintf(stderr, "Unable to release unit_lock : %d , %s .\n",
                        rc, strerror(rc));
                pthread_exit(NULL);
            }

            printf("Ten is already reached !\n");
            pthread_exit(NULL);
        }

        /* update nb unit */
        printf("From %d to %d .\n", *abacus->nb_unit, *abacus->nb_unit + 1);
        *abacus->nb_unit = *abacus->nb_unit + 1;

        /* check if ten is reached */
        if (*abacus->nb_unit == 10) {
            rc = pthread_mutex_unlock(abacus->unit_lock);
            if (rc) {
                fprintf(stderr, "Unable to release unit_lock : %d , %s .\n",
                        rc, strerror(rc));
                pthread_exit(NULL);
            }

            printf("Winner! I reached ten.\n");
            pthread_exit(NULL);
        }

        /* release unit lock */
        rc = pthread_mutex_unlock(abacus->unit_lock);
        if (rc) {
            fprintf(stderr, "Unable to release unit_lock: %d , %s .\n",
                    rc, strerror(rc));
            pthread_exit(NULL);
        }
    } while (0 == 0);
}

int main(int argc, char **argv)
{
    pthread_mutex_t unit_lock = PTHREAD_MUTEX_INITIALIZER;
    unsigned int nb_unit = 0;
    struct shared_abacus abacus;
    pthread_t thread_id[NB_THREAD];
    int rc;
    int i;

    /* init thread input */
    abacus.nb_unit = &nb_unit;
    abacus.unit_lock = &unit_lock;

    /* create threads */
    for (i = 0; i < NB_THREAD; i++) {
        rc = pthread_create(&thread_id[i], NULL, counter_to_ten, &abacus);
        if (rc) {
            fprintf(stderr, "Error on creating thread %d : %d , %s .\n",
                    i, rc, strerror(rc));
            exit(EXIT_FAILURE);
        }
    }

    /* join threads */
    for (i = 0; i < NB_THREAD; i++) {
        rc = pthread_join(thread_id[i], NULL);
        if (rc) {
            fprintf(stderr, "Error on joining thread %d : %d , %s .\n",
                    i, rc, strerror(rc));
            exit(EXIT_FAILURE);
        }
    }

    exit(EXIT_SUCCESS);
}
