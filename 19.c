#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

int num[50];
int n;
double mean,median,std_dev;

void *mean_fn(void *args) {
    int sum = 0;
    for(int i=0 ; i<n ; i++) {
        sum += num[i];
    }
    mean = (double)sum / n;
    pthread_exit(NULL);
}

void *median_fn(void *args) {
    int temp;
    for(int i=0; i<n-1 ; i++) {
        for(int j=0 ; j<n-i-1 ; j++) {
            if(num[j] > num[j+1]) {
                temp = num[j];
                num[j] = num[j+1];
                num[j+1] = temp;
            }
        }
    }

    if(n%2 == 0) {
        median = (num[n/2 -1] + num[n/2])/2.0;
    }
    else {
        median = (double) num[n/2];
    }
    pthread_exit(NULL);
}

void *std_dev_fn(void *args) {
    double local_mean;
    int sum = 0,diff;
    for(int i=0 ; i<n ; i++) {
        sum += num[i];
    }
    local_mean = (double)sum / n;
    sum = 0;
    for(int i=0 ; i<n ; i++) {
        diff = mean - num[i];
        sum += diff*diff;
    }
    std_dev = sqrt(sum/n);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    pthread_t t1,t2,t3;
    n = argc - 1;
    if(argc < 2) {
        printf("Pass the integers as arguments\n");
        exit(0);
    }

    for(int i=0 ; i<n ; i++) {
        num[i] = atoi(argv[i+1]);
    }

    pthread_create(&t1,NULL,mean_fn,NULL);
    pthread_create(&t2,NULL,median_fn,NULL);
    pthread_create(&t3,NULL,std_dev_fn,NULL);

    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    pthread_join(t3,NULL);

    printf("Mean : %.2f\n",mean);
    printf("Median : %.2f\n",median);
    printf("Standard Deviation : %.2f\n",std_dev);

    return 0;
}