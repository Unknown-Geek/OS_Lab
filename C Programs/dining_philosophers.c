#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5
#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define LEFT (phnum + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS
#define RIGHT (phnum + 1) % NUM_PHILOSOPHERS

// Semaphores
sem_t mutex;                        // Mutex for critical sections
sem_t philosopher_sem[NUM_PHILOSOPHERS];  // One semaphore per philosopher

// State of each philosopher
int state[NUM_PHILOSOPHERS];

// Thread function prototype
void *philosopher(void *arg);
void take_forks(int phnum);
void put_forks(int phnum);
void test(int phnum);
void think(int phnum);
void eat(int phnum);

int main() {
    pthread_t threads[NUM_PHILOSOPHERS];
    int philosopher_ids[NUM_PHILOSOPHERS];
    
    // Initialize semaphores
    sem_init(&mutex, 0, 1);
    
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_init(&philosopher_sem[i], 0, 0);
    }
    
    printf("Dining Philosophers Problem Simulation\n");
    printf("======================================\n");
    
    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosopher_ids[i] = i;
        pthread_create(&threads[i], NULL, philosopher, &philosopher_ids[i]);
        
        printf("Philosopher %d is thinking\n", i + 1);
    }
    
    // Wait for all philosophers to finish
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    // Destroy semaphores
    sem_destroy(&mutex);
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_destroy(&philosopher_sem[i]);
    }
    
    return 0;
}

// Philosopher thread function
void *philosopher(void *arg) {
    int phnum = *((int *)arg);
    
    while (1) {
        think(phnum);
        take_forks(phnum);
        eat(phnum);
        put_forks(phnum);
    }
    
    return NULL;
}

// Function for philosopher to think
void think(int phnum) {
    printf("Philosopher %d is thinking\n", phnum + 1);
    sleep(rand() % 3 + 1); // Think for a random period
}

// Function for philosopher to eat
void eat(int phnum) {
    printf("Philosopher %d is eating\n", phnum + 1);
    sleep(rand() % 3 + 1); // Eat for a random period
}

// Take forks - try to acquire both forks
void take_forks(int phnum) {
    sem_wait(&mutex);
    
    // Indicate that philosopher is hungry
    state[phnum] = HUNGRY;
    printf("Philosopher %d is hungry\n", phnum + 1);
    
    // Try to acquire both forks
    test(phnum);
    
    sem_post(&mutex);
    
    // Wait if unable to acquire both forks
    sem_wait(&philosopher_sem[phnum]);
}

// Put down forks - release both forks
void put_forks(int phnum) {
    sem_wait(&mutex);
    
    // Indicate that philosopher is thinking
    state[phnum] = THINKING;
    printf("Philosopher %d putting forks down\n", phnum + 1);
    
    // Check if neighbors can now eat
    test(LEFT);
    test(RIGHT);
    
    sem_post(&mutex);
}

// Test if philosopher can eat
void test(int phnum) {
    if (state[phnum] == HUNGRY && 
        state[LEFT] != EATING && 
        state[RIGHT] != EATING) {
        
        // Philosopher can eat
        state[phnum] = EATING;
        
        printf("Philosopher %d picked up forks\n", phnum + 1);
        
        // Signal that philosopher can eat
        sem_post(&philosopher_sem[phnum]);
    }
}