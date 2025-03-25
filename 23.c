#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>

#define N 5
#define LEFT (i + N - 1) % N
#define RIGHT (i + 1) % N
#define THINK 0
#define HUNGRY 1
#define EATING 2

pthread_mutex_t mutex;
sem_t s[N];
int state[N];

void check(int i)
{
    if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
    {
        state[i] = EATING;
        sem_post(&s[i]);
    }
}

void pick_chopsticks(int i)
{
    pthread_mutex_lock(&mutex);
    state[i] = HUNGRY; // Changed from EATING to HUNGRY
    check(i);
    pthread_mutex_unlock(&mutex);
    sem_wait(&s[i]);
}

void return_chopsticks(int i)
{
    pthread_mutex_lock(&mutex);
    state[i] = THINK;
    check(LEFT);
    check(RIGHT);
    pthread_mutex_unlock(&mutex);
}

void *philosopher(void *args)
{
    int id = *((int *)args);
    while(1)
    {
        printf("Philosopher %d is thinking\n", id);
        sleep(1);

        printf("Philosopher %d is hungry\n", id);
        pick_chopsticks(id);

        printf("Philosopher %d is eating\n", id);
        sleep(2);

        return_chopsticks(id);
    }
    pthread_exit(NULL);
}

int main()
{
    pthread_t phil[N];
    int id[5];

    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < N; i++)
    {
        sem_init(&s[i], 0, 0);
    }

    for (int i = 0; i < N; i++)
    {
        id[i] = i;
        pthread_create(&phil[i], NULL, philosopher, &id[i]);
    }

    for (int i = 0; i < N; i++)
    {
        pthread_join(phil[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    for (int i = 0; i < N; i++)
    {
        sem_destroy(&s[i]);
    }

    return 0;
}