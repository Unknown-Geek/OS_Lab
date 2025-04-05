#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define SIZE 5
#define PROD_NO 3
#define CONS_NO 2

sem_t empty,fill;
pthread_mutex_t mutex;

int in =0, out = 0, count = 0, item_no = 0;
int buffer[SIZE];

void insert(int item) {
    buffer[in] = item;
    in = (in + 1) % SIZE;
    count++;
}

int delete() {
    int item;
    item = buffer[out];
    out = (out + 1) % SIZE;
    count--;
    return item;
}

void *producer(void *args) {
    int item;
    int id = *((int *)args);
     while(1) {
        pthread_mutex_lock(&mutex);
        item = item_no++;
        pthread_mutex_unlock(&mutex);

        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        insert(item);
        pthread_mutex_unlock(&mutex);
        printf("Producer %d produced item %d, Buffer Count : %d\n",id,item,count);
        sleep(1);
        sem_post(&fill);
     }
}

void *consumer(void *args) {
    int item;
    int id = *((int *)args);
     while(1) {
        sem_wait(&fill);
        pthread_mutex_lock(&mutex);
        item = delete();
        pthread_mutex_unlock(&mutex);
        printf("Consumer %d consumed item %d, Buffer Count : %d\n",id,item,count);
        sleep(1);
        sem_post(&empty);
     }
}

int main() {
    pthread_t producers[PROD_NO];
    pthread_t consumers[CONS_NO];

    int prod_id[PROD_NO];
    int cons_id[CONS_NO];

    sem_init(&empty,0,SIZE);
    sem_init(&fill,0,0);
    pthread_mutex_init(&mutex,NULL);

    for(int i=0 ; i<PROD_NO ; i++) {
        prod_id[i] = i;
        pthread_create(&producers[i],NULL,producer,&prod_id[i]);
    }

    for(int i=0 ; i<CONS_NO ; i++) {
        cons_id[i] = i;
        pthread_create(&consumers[i],NULL,consumer,&cons_id[i]);
    }

    for(int i=0 ; i<PROD_NO ; i++) {
        pthread_join(producers[i],NULL);
    }

    for(int i=0 ; i<CONS_NO ; i++) {
        pthread_join(consumers[i],NULL);
    }

    sem_destroy(&empty);
    sem_destroy(&fill);
    pthread_mutex_destroy(&mutex);

    return 0;
}