#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid1, pid2;
    int status;
    
    printf("Parent process (PID: %d) started\n", getpid());
    
    // Create first child process
    pid1 = fork();
    
    if (pid1 < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid1 == 0) {
        // Code executed by first child process
        printf("First Child Process (PID: %d, Parent PID: %d) started\n", 
               getpid(), getppid());
        
        // Use exec to run ls command
        printf("First Child: Executing 'ls -l' command...\n");
        execlp("ls", "ls", "-l", NULL);
        
        // If exec returns, it's an error
        perror("Exec failed");
        exit(1);
    } else {
        // Parent process continues
        printf("Parent: Created first child with PID %d\n", pid1);
        
        // Create second child process
        pid2 = fork();
        
        if (pid2 < 0) {
            perror("Fork failed");
            exit(1);
        } else if (pid2 == 0) {
            // Code executed by second child process
            printf("Second Child Process (PID: %d, Parent PID: %d) started\n", 
                   getpid(), getppid());
            
            // Sleep for a short time
            sleep(1);
            
            // Use exec to run the date command
            printf("Second Child: Executing 'date' command...\n");
            execlp("date", "date", NULL);
            
            // If exec returns, it's an error
            perror("Exec failed");
            exit(1);
        } else {
            // Parent process continues
            printf("Parent: Created second child with PID %d\n", pid2);
            
            // Wait for first child to complete
            waitpid(pid1, &status, 0);
            if (WIFEXITED(status)) {
                printf("Parent: First child exited with status %d\n", WEXITSTATUS(status));
            } else {
                printf("Parent: First child did not exit normally\n");
            }
            
            // Wait for second child to complete
            waitpid(pid2, &status, 0);
            if (WIFEXITED(status)) {
                printf("Parent: Second child exited with status %d\n", WEXITSTATUS(status));
            } else {
                printf("Parent: Second child did not exit normally\n");
            }
            
            // Create a child process that executes another program
            pid_t pid3 = fork();
            
            if (pid3 < 0) {
                perror("Fork failed");
                exit(1);
            } else if (pid3 == 0) {
                // Third child process
                printf("Third Child Process (PID: %d) started\n", getpid());
                printf("Third Child: Executing 'ps' command...\n");
                
                // Execute the ps command with arguments
                char *args[] = {"ps", "-f", NULL};
                execvp("ps", args);
                
                // If exec returns, it's an error
                perror("Exec failed");
                exit(1);
            } else {
                // Wait for third child to complete
                waitpid(pid3, &status, 0);
                if (WIFEXITED(status)) {
                    printf("Parent: Third child exited with status %d\n", WEXITSTATUS(status));
                } else {
                    printf("Parent: Third child did not exit normally\n");
                }
            }
            
            printf("Parent process completed\n");
        }
    }
    
    return 0;
}