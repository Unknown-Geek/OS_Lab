#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_REQUESTS 100

// Function prototypes
void fcfs(int requests[], int n, int head);
void scan(int requests[], int n, int head, int disk_size);
void cscan(int requests[], int n, int head, int disk_size);
void look(int requests[], int n, int head);
void sort_requests(int requests[], int n);

int main() {
    int n, head, disk_size;
    int requests[MAX_REQUESTS];
    
    printf("Enter the number of disk requests: ");
    scanf("%d", &n);
    
    printf("Enter the request sequence: ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &requests[i]);
    }
    
    printf("Enter the initial head position: ");
    scanf("%d", &head);
    
    printf("Enter the disk size (0 to size-1): ");
    scanf("%d", &disk_size);
    
    printf("\n");
    fcfs(requests, n, head);
    
    printf("\n");
    scan(requests, n, head, disk_size);
    
    printf("\n");
    cscan(requests, n, head, disk_size);
    
    printf("\n");
    look(requests, n, head);
    
    return 0;
}

// First Come First Serve disk scheduling
void fcfs(int requests[], int n, int head) {
    int total_head_movement = 0;
    int current_head = head;
    
    printf("FCFS Disk Scheduling Algorithm\n");
    printf("------------------------------\n");
    printf("Request Sequence: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", requests[i]);
    }
    printf("\n");
    
    printf("\nHead Movement:\n");
    printf("%d", head);
    
    for (int i = 0; i < n; i++) {
        printf(" -> %d", requests[i]);
        total_head_movement += abs(requests[i] - current_head);
        current_head = requests[i];
    }
    
    printf("\n\nTotal head movement: %d cylinders\n", total_head_movement);
    printf("Average head movement: %.2f cylinders\n", (float)total_head_movement / n);
}

// SCAN (Elevator) disk scheduling
void scan(int requests[], int n, int head, int disk_size) {
    int total_head_movement = 0;
    int current_head = head;
    int temp_requests[MAX_REQUESTS + 2]; // +2 for head and disk end
    int temp_n = n + 2;
    
    // Copy requests and add head position
    for (int i = 0; i < n; i++) {
        temp_requests[i] = requests[i];
    }
    temp_requests[n] = head;
    temp_requests[n+1] = disk_size - 1; // Add disk end
    
    // Sort requests
    for (int i = 0; i < temp_n; i++) {
        for (int j = 0; j < temp_n - i - 1; j++) {
            if (temp_requests[j] > temp_requests[j+1]) {
                int temp = temp_requests[j];
                temp_requests[j] = temp_requests[j+1];
                temp_requests[j+1] = temp;
            }
        }
    }
    
    // Find head position in the sorted array
    int head_index = 0;
    for (int i = 0; i < temp_n; i++) {
        if (temp_requests[i] == head) {
            head_index = i;
            break;
        }
    }
    
    printf("SCAN (Elevator) Disk Scheduling Algorithm\n");
    printf("----------------------------------------\n");
    printf("Request Sequence: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", requests[i]);
    }
    printf("\n");
    
    printf("\nHead Movement:\n");
    printf("%d", head);
    
    // Move towards higher cylinder numbers
    for (int i = head_index + 1; i < temp_n; i++) {
        printf(" -> %d", temp_requests[i]);
        total_head_movement += abs(temp_requests[i] - current_head);
        current_head = temp_requests[i];
    }
    
    // Move towards lower cylinder numbers
    for (int i = head_index - 1; i >= 0; i--) {
        printf(" -> %d", temp_requests[i]);
        total_head_movement += abs(temp_requests[i] - current_head);
        current_head = temp_requests[i];
    }
    
    printf("\n\nTotal head movement: %d cylinders\n", total_head_movement);
    printf("Average head movement: %.2f cylinders\n", (float)total_head_movement / n);
}

// C-SCAN disk scheduling
void cscan(int requests[], int n, int head, int disk_size) {
    int total_head_movement = 0;
    int current_head = head;
    int temp_requests[MAX_REQUESTS + 3]; // +3 for head, disk start, and disk end
    int temp_n = n + 3;
    
    // Copy requests and add head position
    for (int i = 0; i < n; i++) {
        temp_requests[i] = requests[i];
    }
    temp_requests[n] = head;
    temp_requests[n+1] = 0;         // Add disk start
    temp_requests[n+2] = disk_size - 1; // Add disk end
    
    // Sort requests
    for (int i = 0; i < temp_n; i++) {
        for (int j = 0; j < temp_n - i - 1; j++) {
            if (temp_requests[j] > temp_requests[j+1]) {
                int temp = temp_requests[j];
                temp_requests[j] = temp_requests[j+1];
                temp_requests[j+1] = temp;
            }
        }
    }
    
    // Find head position in the sorted array
    int head_index = 0;
    for (int i = 0; i < temp_n; i++) {
        if (temp_requests[i] == head) {
            head_index = i;
            break;
        }
    }
    
    printf("C-SCAN Disk Scheduling Algorithm\n");
    printf("---------------------------------\n");
    printf("Request Sequence: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", requests[i]);
    }
    printf("\n");
    
    printf("\nHead Movement:\n");
    printf("%d", head);
    
    // Move towards higher cylinder numbers
    for (int i = head_index + 1; i < temp_n; i++) {
        if (temp_requests[i] != head) {
            printf(" -> %d", temp_requests[i]);
            total_head_movement += abs(temp_requests[i] - current_head);
            current_head = temp_requests[i];
        }
    }
    
    // Jump from end to start
    if (current_head != 0) {
        printf(" -> 0");
        // This movement is not counted in head movement since it's a quick return
        current_head = 0;
    }
    
    // Continue from the beginning up to the head
    for (int i = 0; i < head_index; i++) {
        if (temp_requests[i] != head) {
            printf(" -> %d", temp_requests[i]);
            total_head_movement += abs(temp_requests[i] - current_head);
            current_head = temp_requests[i];
        }
    }
    
    printf("\n\nTotal head movement: %d cylinders\n", total_head_movement);
    printf("Average head movement: %.2f cylinders\n", (float)total_head_movement / n);
}

// LOOK disk scheduling
void look(int requests[], int n, int head) {
    int total_head_movement = 0;
    int current_head = head;
    int temp_requests[MAX_REQUESTS + 1]; // +1 for head
    int temp_n = n + 1;
    
    // Copy requests and add head position
    for (int i = 0; i < n; i++) {
        temp_requests[i] = requests[i];
    }
    temp_requests[n] = head;
    
    // Sort requests
    for (int i = 0; i < temp_n; i++) {
        for (int j = 0; j < temp_n - i - 1; j++) {
            if (temp_requests[j] > temp_requests[j+1]) {
                int temp = temp_requests[j];
                temp_requests[j] = temp_requests[j+1];
                temp_requests[j+1] = temp;
            }
        }
    }
    
    // Find head position in the sorted array
    int head_index = 0;
    for (int i = 0; i < temp_n; i++) {
        if (temp_requests[i] == head) {
            head_index = i;
            break;
        }
    }
    
    printf("LOOK Disk Scheduling Algorithm\n");
    printf("------------------------------\n");
    printf("Request Sequence: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", requests[i]);
    }
    printf("\n");
    
    printf("\nHead Movement:\n");
    printf("%d", head);
    
    // Move towards higher cylinder numbers
    for (int i = head_index + 1; i < temp_n; i++) {
        printf(" -> %d", temp_requests[i]);
        total_head_movement += abs(temp_requests[i] - current_head);
        current_head = temp_requests[i];
    }
    
    // Move towards lower cylinder numbers
    for (int i = head_index - 1; i >= 0; i--) {
        printf(" -> %d", temp_requests[i]);
        total_head_movement += abs(temp_requests[i] - current_head);
        current_head = temp_requests[i];
    }
    
    printf("\n\nTotal head movement: %d cylinders\n", total_head_movement);
    printf("Average head movement: %.2f cylinders\n", (float)total_head_movement / n);
}