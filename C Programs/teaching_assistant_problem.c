#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <semaphore.h>
#include <pthread.h>

sem_t ta_sem, stud_sem;
pthread_mutex_t mutex;
int waiting = 0;

void *ta(void *args) {
    while (1) {
        sem_wait(&ta_sem);
        pthread_mutex_lock(&mutex);
        if (waiting > 0) {
            waiting--;
            printf("TA helping student \n");
            printf("No. of students waiting : %d\n", waiting);
            pthread_mutex_unlock(&mutex);
            sleep(1);
            printf("TA finished helping student\n");
            sem_post(&stud_sem);
        }
        else {
            pthread_mutex_unlock(&mutex);
        }
    }
    pthread_exit(NULL);
}

void *student(void *args) {
    int index = *((int *)args);
    while (1) {
        pthread_mutex_lock(&mutex);
        if (waiting < 3) {
            waiting++;
            printf("Student %d joined the queue\n", index);
            printf("No. of students waiting : %d\n", waiting);
            pthread_mutex_unlock(&mutex);
            sem_post(&ta_sem);
            sem_wait(&stud_sem);
            printf("Student %d got help from TA\n", index);
            sleep(1);
        }
        else {
            printf("No seats left. Student %d leaving\n", index);
            pthread_mutex_unlock(&mutex);
            sleep(1);
        }
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t ta_thread;
    pthread_t students[5];
    int ids[5];
    int len = 5;

    sem_init(&ta_sem, 0, 0);
    sem_init(&stud_sem, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&ta_thread, NULL, ta, NULL);

    for (int i = 0; i < len; i++) {
        ids[i] = i;
        pthread_create(&students[i], NULL, student, &ids[i]);
    }

    for (int i = 0; i < len; i++) {
        pthread_join(students[i], NULL);
    }
    pthread_join(ta_thread, NULL);

    sem_destroy(&ta_sem);
    sem_destroy(&stud_sem);
    pthread_mutex_destroy(&mutex);

    printf("Program terminated successfully\n");

    return 0;
}