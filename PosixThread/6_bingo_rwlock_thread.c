#include <errno.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define NB_PLAYER 4
#define MAX_RANDOM 100
#define TIME_BETWEEN_NEW_NUMBER_S 1
#define TIME_BETWEEN_CHECK_NEW_NUMBER_MS 1

struct board {
    bool started;
    unsigned int new_number;
    bool bingo;
    pthread_rwlock_t *rwlock;
};

void *player(void *_board)
{
    unsigned int my_number = pthread_self() % MAX_RANDOM;
    struct board *board = (struct board *)_board;
    int rc;

    printf("I'm ready to play with the %u number.\n", my_number);
    while (0 == 0) {
        bool ended = false;
        bool win = false;

        rc = pthread_rwlock_rdlock(board->rwlock);
        if (rc) {
            fprintf(stderr,
                    "Error in player when taking read lock : '%d', '%s'.\n",
                    rc, strerror(rc));
            exit(rc);
        }

        if (board->bingo)
            ended = true;

        if (board->started && board->new_number == my_number)
            win = true;

        rc = pthread_rwlock_unlock(board->rwlock);
        if (rc) {
            fprintf(stderr,
                    "Error in player when releasing lock : '%d', '%s'.\n",
                    rc, strerror(rc));
            exit(rc);
        }

        if (ended)
            break;

        if (win) {
            rc = pthread_rwlock_wrlock(board->rwlock);
            if (rc) {
                fprintf(stderr,
                        "Error in thread when taking write lock: '%d', "
                        "'%s'.\n", rc, strerror(rc));
                exit(rc);
            }

            if (!board->bingo) {
                printf("I win with number '%u'.\n", my_number); 
                board->bingo = true;
            }

            rc = pthread_rwlock_unlock(board->rwlock);
            if (rc) {
                fprintf(stderr,
                        "Error in player when releasing lock : '%d', '%s'.\n",
                        rc, strerror(rc));
                exit(rc);
            }

            break;
        }

        if (usleep(TIME_BETWEEN_CHECK_NEW_NUMBER_MS * 1000)) {
            rc = errno;
            fprintf(stderr,
                    "Error in player when sleeping : '%d', '%s'.\n",
                    rc, strerror(rc));
            exit(rc);
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char **argv)
{
    pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
    unsigned int seed = time(NULL);
    pthread_t thread_id[NB_PLAYER];
    struct board board;
    unsigned int i;
    int rc;

    board.started = false;
    board.bingo = false;
    board.rwlock = &rwlock;

    for (i = 0; i < NB_PLAYER; i++) {
        rc = pthread_create(&thread_id[i], NULL, player, &board);
        if (rc) {
            fprintf(stderr,
                    "Error in main thread when creating thread '%i': '%d', "
                    "'%s'.\n", i, rc, strerror(rc));
            exit(rc);
        }
    }

    while (0 == 0) {
        unsigned int new_number = rand_r(&seed) % MAX_RANDOM;

        rc = pthread_rwlock_wrlock(&rwlock);
        if (rc) {
            fprintf(stderr,
                    "Error in main thread when taking write lock: '%d', "
                    "'%s'.\n", rc, strerror(rc));
            exit(rc);
        }

        if (board.bingo) {
            printf("Congrats! We have a winner!\n");

            rc = pthread_rwlock_unlock(&rwlock);
            if (rc) {
                fprintf(stderr,
                        "Error in main thread when releasing lock: '%d', "
                        "'%s'.\n", rc, strerror(rc));
                exit(rc);
            }

            break;
        }

        printf("New number %u .\n", new_number);
        board.new_number = new_number;

        if (!board.started)
            board.started = true;

        rc = pthread_rwlock_unlock(&rwlock);
        if (rc) {
            fprintf(stderr,
                    "Error in main thread when releasing lock: '%d', '%s'.\n",
                    rc, strerror(rc));
            exit(rc);
        }

        sleep(TIME_BETWEEN_NEW_NUMBER_S);
    }

    for (i = 0; i < NB_PLAYER; i++) {
        rc = pthread_join(thread_id[i], NULL);
        if (rc) {
            fprintf(stderr,
                    "Error in main thread when joining thread '%i': '%d', "
                    "'%s'.\n", i, rc, strerror(rc));
            exit(rc);
        }
    }

    exit(EXIT_SUCCESS);
}
