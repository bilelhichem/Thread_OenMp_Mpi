#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NB_THREAD 4

struct thread_param {
    double toto;
    int *tab_titi;
/*
    [... other parameters ...]
    [...]
*/
};

void * thread_function(void *_arg)
{
    struct thread_param *arg = (struct thread_param *)_arg;
    double internal_toto = arg->toto;
    int *internal_tab_titi = arg->tab_titi;
/*
    [...]
    [...]
*/
    pthread_exit(externally_valid_ptr_as_result);
    /* or pthread_exit(NULL) if no returned pointer */
}


int main(int argc,char **argv)
{
/*
    [...]
    [...]
*/
    struct thread_param thread_arg[NB_THREAD];
    void *returned_pointer[NB_THREAD];
    pthread_t thread_id[NB_THREAD];
    int *common_tab;
    int rc = 0;
    int i;

/*
    [...]
    [...]
*/

    /* creating threads */
    for(i = 0; i < NB_THREAD; i++) {
        /* thread param initialization */
        thread_arg[i].toto = 3*34 + 99 - 128 + 77 * i;
        thread_arg[i].tab_titi = common_tab;
/*
        [...]
        [...]
*/
        /* launch thread */
        rc = pthread_create(&thread_id[i], NULL, thread_function,
                            &thread_arg[i]);
        if (rc) {
            fprintf(stderr, "Error when creating thread %d, return code: %d .\n",
                    i, rc);
            exit(EXIT_FAILURE);
        }
    }

    /* joining threads (and getting returned pointer if needed) */
    for(i = 0; i < NB_THREAD; i++) {
        rc = pthread_join(thread_id[i], &returned_pointer[i]);
        if (rc) {
            fprintf(stderr, "Error when joining thread %d, return code: %d .\n",
                    i, rc);
            exit(EXIT_FAILURE);
        }
    }

/*
    [...]
    [...]
*/

    /* successfull end */
    exit(EXIT_SUCCESS);
}
