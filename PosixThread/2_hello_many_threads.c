#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NB_THREAD 50

struct printer_input {
    char *sentence;
    unsigned int id;
};

void *printer_with_id(void *_input)
{
    struct printer_input *input = (struct printer_input *)_input;
    printf("%s: from thread %d.\n", input->sentence, input->id);
    pthread_exit(NULL);
}

int main(int argc, char **argv)
{
    struct printer_input thread_input[NB_THREAD];
    pthread_t thread_id[NB_THREAD];
    unsigned int i;
    int rc;

    /* create threads */
    for (i = 0; i < NB_THREAD; i++) {
        thread_input[i].sentence = "Hello world";
        thread_input[i].id = i;
        rc = pthread_create(&thread_id[i], NULL, printer_with_id,
                            &thread_input[i]);
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
