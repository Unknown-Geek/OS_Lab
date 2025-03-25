#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    // Check if command is provided
    if (argc < 2) {
        printf("Usage: exectime command [args...]\n");
        exit(1);
    }

    // Create shared memory segment for timestamp
    int shmid = shmget(IPC_PRIVATE, sizeof(struct timeval), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget failed");
        exit(1);
    }

    // Attach shared memory - still need a pointer here
    struct timeval *start_time = (struct timeval *)shmat(shmid, NULL, 0);
    if (start_time == (struct timeval *)-1) {
        perror("shmat failed");
        exit(1);
    }

    // Fork a child process
    pid_t pid = fork();

    if (pid < 0) {
        // Error
        perror("fork failed");
        exit(1);
    } else if (pid == 0) {
        // Child process

        // Record start time using array indexing
        gettimeofday(&start_time[0], NULL);

        // Execute the command
        execvp(argv[1], &argv[1]);
        
        // If execvp returns, it failed
        perror("execvp failed");
        exit(1);
    } else {
        // Parent process
        int status;
        
        // Wait for child to finish
        wait(&status);
        
        // Record end time
        struct timeval end_time;
        gettimeofday(&end_time, NULL);
        
        // Calculate elapsed time using array indexing
        long seconds = end_time.tv_sec - start_time[0].tv_sec;
        long microseconds = end_time.tv_usec - start_time[0].tv_usec;
        
        if (microseconds < 0) {
            seconds--;
            microseconds += 1000000;
        }
        
        // Print elapsed time
        printf("Elapsed time: %ld.%06ld seconds\n", seconds, microseconds);
        
        // Detach and remove shared memory
        shmdt(start_time);
        shmctl(shmid, IPC_RMID, NULL);
    }
    
    return 0;
}