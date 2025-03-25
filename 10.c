#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    pid_t pid;
    pid = fork();
    char command[50];

    if (pid == 0) {
        printf("I am %d and my parent is %d\n", getpid(), getppid());
        sprintf(command, "pstree -p -s %d -H %d", getpid(), getpid());
        execlp("sh", "sh", "-c", command, NULL);
    }
    else {
        printf("I am %d and my child is %d\n", getpid(), pid);
        wait(NULL);
    }
    return 0;
}