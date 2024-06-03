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
        printf("Filozof %d uzima vilice i jede\n", br + 1);
        sleep(1);
        sem_post(&sem[br]);
    }
}

void uzima_vilicu(int br) {
    sem_wait(&mutex);
    state[br] = 1;
    printf("Filozof %d je gladan\n", br + 1);
    test(br);
    sem_post(&mutex);
    sem_wait(&sem[br]);
    sleep(1);
}

void vrati_vilice(int br) {
    sem_wait(&mutex);
    printf("Filozof %d ostavlja vilice i ide misliti\n", br + 1);
    state[br] = 3;
    test((br + 4) % N);
    test((br + 1) % N);
    sem_post(&mutex);
    sleep(1);
}

void* filozof(void* pdr) {
    int br = *((int *)pdr);
    while (1) {
        sleep(2);
        uzima_vilicu(br);
        sleep(0);
        vrati_vilice(br);
    }
}

int main() {
    pthread_t thread_id[N];
    sem_init(&mutex, 0, 1);
    for (int i = 0; i < N; i++) {
        sem_init(&sem[i], 0, 0);
        state[i] = 3;
    }
    for (int i = 0; i < N; i++) {
        pthread_create(&thread_id[i], NULL, filozof, &fil_br[i]);
        printf("Filozof %d misli\n", i + 1);
    }
    for (int i = 0; i < N; i++) {
        pthread_join(thread_id[i], NULL);
    }
    return 0;
}
