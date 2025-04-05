#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    int n = 10;
    int a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int odd[10], even[10];
    int oddCount = 0, evenCount = 0;
    pid_t pid;

    pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        return 1;
    }
    else if (pid == 0) {
        // Child process - find odd numbers
        for (int i = 0; i < n; i++) {
            if (a[i] % 2 != 0) {
                odd[oddCount++] = a[i];
            }
        }
        printf("Child process: Odd numbers: ");
        for (int i = 0; i < oddCount; i++) {
            printf("%d ", odd[i]);
        }
        printf("\n");
    }
    else {
        // Parent process - find even numbers
        wait(NULL);  // Wait for child to complete
        for (int i = 0; i < n; i++) {
            if (a[i] % 2 == 0) {
                even[evenCount++] = a[i];
            }
        }
        printf("Parent process: Even numbers: ");
        for (int i = 0; i < evenCount; i++) {
            printf("%d ", even[i]);
        }
        printf("\n");
    }

    return 0;
}