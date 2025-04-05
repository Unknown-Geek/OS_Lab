#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    int num1,num2;
    char command[50];
    printf("Enter the two numbers : ");
    scanf("%d%d",&num1,&num2);

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }

    if(pid == 0) {
        sprintf(command,"./simple_adder %d %d",num1,num2);
        execlp("sh","sh","-c",command,NULL);
        perror("execlp failed");
        exit(1);
    }
    else {
        wait(NULL);
    }
    return 0;
}