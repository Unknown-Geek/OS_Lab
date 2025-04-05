#include <stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>

#define MAX 50

void odd(int N,int num[MAX]) {
    printf("Odd Numbers : \n");
    for(int i=0 ; i<N ; i++) {
        if(num[i]%2 != 0)   
            printf("%d\t",num[i]);
    }
    printf("\n");
}

void even(int N,int num[MAX]) {
    printf("Even Numbers : \n");
    for(int i=0 ; i<N ; i++) {
        if(num[i]%2 == 0)   
            printf("%d\t",num[i]);
    }
    printf("\n");
}

int main() {
    int N;
    int num[MAX];
    pid_t pid1,pid2;

    printf("Enter the number of entires : ");
    scanf("%d",&N);
    printf("\n");

    printf("Enter the numbers : \n");
    for(int i=0 ; i<N ; i++) {
        scanf("%d",&num[i]);
    }
    printf("\n");

    pid1 = fork();
    if(pid1 == 0) {
        odd(N,num);
        exit(0);
    }
    else {
        waitpid(pid1,NULL,0);
    }

    pid2 = fork();
    if(pid2 == 0) {
        even(N,num);
        exit(0);
    }
    else {
        waitpid(pid2,NULL,0);
    }
    
    return 0;
}