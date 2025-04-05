#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: exectime command [args...]\n");
        exit(1);
    }

    int shmid = shmget(IPC_PRIVATE, sizeof(struct timeval), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget failed");
        exit(1);
    }

    struct timeval *start_time = (struct timeval *)shmat(shmid, NULL, 0);
    if (start_time == (struct timeval *)-1) {
        perror("shmat failed");
        exit(1);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }
    else if (pid == 0) {
        gettimeofday(&start_time[0], NULL);

        execvp(argv[1], &argv[1]);

        perror("execvp failed");
        exit(1);
    }
    else {
        int status;

        wait(&status);

        struct timeval end_time;
        gettimeofday(&end_time, NULL);

        long seconds = end_time.tv_sec - start_time[0].tv_sec;
        long microseconds = end_time.tv_usec - start_time[0].tv_usec;

        if (microseconds < 0) {
            seconds--;
            microseconds += 1000000;
        }

        printf("Elapsed time: %ld.%06ld seconds\n", seconds, microseconds);

        shmdt(start_time);
        shmctl(shmid, IPC_RMID, NULL);
    }

    return 0;
}