#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define SHM_SIZE 1024  // Size of shared memory segment in bytes

// Structure to be stored in shared memory
typedef struct {
    int status;          // 0 = empty, 1 = filled, 2 = processed
    char message[256];   // Message data
} SharedData;

int main() {
    int shmid;
    key_t key = 1234;  // IPC key for shared memory
    SharedData *shared_data;
    pid_t pid;
    
    // Create shared memory segment
    if ((shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    
    // Attach the shared memory segment
    if ((shared_data = shmat(shmid, NULL, 0)) == (SharedData *) -1) {
        perror("shmat");
        exit(1);
    }
    
    // Initialize shared memory
    shared_data->status = 0;  // Empty
    
    printf("Shared memory created and attached.\n");
    printf("Shared Memory ID: %d\n", shmid);
    
    // Create child process
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }
    
    if (pid == 0) {
        // Child process (consumer)
        printf("\nChild process started (PID: %d). Waiting for data...\n", getpid());
        
        // Wait for parent to write data
        while (shared_data->status != 1) {
            sleep(1);
        }
        
        // Read data from shared memory
        printf("\nChild process received data: %s\n", shared_data->message);
        
        // Process the data (just convert to uppercase in this example)
        for (int i = 0; shared_data->message[i]; i++) {
            if (shared_data->message[i] >= 'a' && shared_data->message[i] <= 'z') {
                shared_data->message[i] = shared_data->message[i] - 32;
            }
        }
        
        // Mark as processed
        shared_data->status = 2;
        printf("Child process processed the data.\n");
        
        // Detach shared memory
        if (shmdt(shared_data) < 0) {
            perror("shmdt");
            exit(1);
        }
        
        printf("Child process detached from shared memory.\n");
        exit(0);
    } else {
        // Parent process (producer)
        printf("\nParent process started (PID: %d).\n", getpid());
        printf("Enter a message to send to child process: ");
        
        // Read message from user
        fgets(shared_data->message, 256, stdin);
        shared_data->message[strcspn(shared_data->message, "\n")] = 0;  // Remove newline
        
        // Set status to filled
        shared_data->status = 1;
        printf("Parent process wrote data to shared memory.\n");
        
        // Wait for child to process data
        while (shared_data->status != 2) {
            sleep(1);
        }
        
        // Read processed data
        printf("\nParent process received processed data: %s\n", shared_data->message);
        
        // Wait for child to exit
        wait(NULL);
        
        // Detach shared memory
        if (shmdt(shared_data) < 0) {
            perror("shmdt");
            exit(1);
        }
        
        // Delete shared memory segment
        if (shmctl(shmid, IPC_RMID, NULL) < 0) {
            perror("shmctl");
            exit(1);
        }
        
        printf("Parent process detached and removed shared memory.\n");
    }
    
    return 0;
}