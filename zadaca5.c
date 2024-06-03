#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#define N 5

int fil_br[N] = {0, 1, 2, 3, 4}; 
sem_t sem[N];
sem_t mutex;
int state[N];

void test(int br) {
    if (state[br] == 1 && state[(br + 4) % N] != 2 && state[(br + 1) % N] != 2) {
        state[br] = 2;
        printf("Philosopher %d takes forks and starts eating\n", br + 1);
        sem_post(&sem[br]);
    }
}

void uzima_vilicu(int br) {
    sem_wait(&mutex);
    state[br] = 1;
    printf("Philosopher %d is hungry\n", br + 1);
    test(br);
    sem_post(&mutex);
    sem_wait(&sem[br]);
}

void vrati_vilice(int br) {
    sem_wait(&mutex);
    printf("Philosopher %d puts down forks and starts thinking\n", br + 1);
    state[br] = 3;
    test((br + 4) % N);
    test((br + 1) % N);
    sem_post(&mutex);
}

void* filozof(void* pdr) {
    int br = *((int *)pdr);
    free(pdr);  // Free allocated memory

    while (1) {
        sleep(2);
        uzima_vilicu(br);
        sleep(1);
        vrati_vilice(br);
    }
}

int main() {
    pthread_t thread_id[N];
    sem_init(&mutex, 0, 1);

    for (int i = 0; i < N; i++) {
        sem_init(&sem[i], 0, 0);
        state[i] = 3; // Philosophers start thinking
    }

    for (int i = 0; i < N; i++) {
        int *br = malloc(sizeof(int));
        *br = i;
        pthread_create(&thread_id[i], NULL, filozof, br);
        printf("Philosopher %d is thinking\n", i + 1);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(thread_id[i], NULL);
    }

    for (int i = 0; i < N; i++) {
        sem_destroy(&sem[i]);
    }
    sem_destroy(&mutex);

    return 0;
}
