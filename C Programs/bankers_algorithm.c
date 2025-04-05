#include <stdio.h>
#include <stdbool.h>

#define MAX_RESOURCES 5
#define MAX_PROCESSES 5

void print_state(int available[], int max[][MAX_RESOURCES], int allocation[][MAX_RESOURCES], 
                int need[][MAX_RESOURCES], int n_processes, int n_resources);
bool is_safe_state(int available[], int max[][MAX_RESOURCES], int allocation[][MAX_RESOURCES], 
                  int need[][MAX_RESOURCES], int n_processes, int n_resources);
bool request_resources(int process_id, int request[], int available[], int max[][MAX_RESOURCES], 
                      int allocation[][MAX_RESOURCES], int need[][MAX_RESOURCES], 
                      int n_processes, int n_resources);

int main() {
    int n_processes, n_resources;
    
    printf("Enter number of processes: ");
    scanf("%d", &n_processes);
    
    printf("Enter number of resource types: ");
    scanf("%d", &n_resources);
    
    // Available resources
    int available[MAX_RESOURCES];
    printf("Enter available resources: ");
    for (int i = 0; i < n_resources; i++) {
        scanf("%d", &available[i]);
    }
    
    // Maximum demand of each process
    int max[MAX_PROCESSES][MAX_RESOURCES];
    printf("Enter maximum demand of each process:\n");
    for (int i = 0; i < n_processes; i++) {
        printf("For process %d: ", i);
        for (int j = 0; j < n_resources; j++) {
            scanf("%d", &max[i][j]);
        }
    }
    
    // Allocated resources for each process
    int allocation[MAX_PROCESSES][MAX_RESOURCES];
    printf("Enter allocation for each process:\n");
    for (int i = 0; i < n_processes; i++) {
        printf("For process %d: ", i);
        for (int j = 0; j < n_resources; j++) {
            scanf("%d", &allocation[i][j]);
            // Update available resources
            available[j] -= allocation[i][j];
        }
    }
    
    // Calculate need matrix
    int need[MAX_PROCESSES][MAX_RESOURCES];
    for (int i = 0; i < n_processes; i++) {
        for (int j = 0; j < n_resources; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }
    
    // Print initial state
    printf("\nInitial State:\n");
    print_state(available, max, allocation, need, n_processes, n_resources);
    
    // Check if system is in safe state
    if (is_safe_state(available, max, allocation, need, n_processes, n_resources)) {
        printf("\nSystem is in SAFE state.\n");
    } else {
        printf("\nSystem is in UNSAFE state.\n");
    }
    
    // Process resource request
    int process_id;
    int request[MAX_RESOURCES];
    
    printf("\nEnter process ID for resource request (0-%d): ", n_processes - 1);
    scanf("%d", &process_id);
    
    printf("Enter resource request for process %d: ", process_id);
    for (int i = 0; i < n_resources; i++) {
        scanf("%d", &request[i]);
    }
    
    if (request_resources(process_id, request, available, max, allocation, need, n_processes, n_resources)) {
        printf("\nResource request granted.\n");
        printf("\nNew State after allocation:\n");
        print_state(available, max, allocation, need, n_processes, n_resources);
        
        if (is_safe_state(available, max, allocation, need, n_processes, n_resources)) {
            printf("\nSystem is in SAFE state after allocation.\n");
        } else {
            printf("\nSystem is in UNSAFE state after allocation.\n");
        }
    } else {
        printf("\nResource request denied. Would cause deadlock.\n");
    }
    
    return 0;
}

// Print current system state
void print_state(int available[], int max[][MAX_RESOURCES], int allocation[][MAX_RESOURCES], 
                int need[][MAX_RESOURCES], int n_processes, int n_resources) {
    printf("\nAvailable Resources: ");
    for (int i = 0; i < n_resources; i++) {
        printf("%d ", available[i]);
    }
    
    printf("\n\nAllocation Matrix:\t\tMax Matrix:\t\tNeed Matrix:\n");
    for (int i = 0; i < n_processes; i++) {
        printf("P%d | ", i);
        
        // Allocation
        for (int j = 0; j < n_resources; j++) {
            printf("%d ", allocation[i][j]);
        }
        
        printf("\t\tP%d | ", i);
        
        // Max
        for (int j = 0; j < n_resources; j++) {
            printf("%d ", max[i][j]);
        }
        
        printf("\t\tP%d | ", i);
        
        // Need
        for (int j = 0; j < n_resources; j++) {
            printf("%d ", need[i][j]);
        }
        
        printf("\n");
    }
}

// Check if system is in safe state
bool is_safe_state(int available[], int max[][MAX_RESOURCES], int allocation[][MAX_RESOURCES], 
                  int need[][MAX_RESOURCES], int n_processes, int n_resources) {
    int work[MAX_RESOURCES];
    bool finish[MAX_PROCESSES] = {false};
    int safe_sequence[MAX_PROCESSES];
    
    // Copy available resources to work
    for (int i = 0; i < n_resources; i++) {
        work[i] = available[i];
    }
    
    // Find processes that can finish
    int count = 0;
    while (count < n_processes) {
        bool found = false;
        
        for (int i = 0; i < n_processes; i++) {
            if (!finish[i]) {
                int j;
                for (j = 0; j < n_resources; j++) {
                    if (need[i][j] > work[j])
                        break;
                }
                
                if (j == n_resources) {
                    // Process can finish, add resources back to work
                    for (int k = 0; k < n_resources; k++) {
                        work[k] += allocation[i][k];
                    }
                    
                    safe_sequence[count] = i;
                    finish[i] = true;
                    count++;
                    found = true;
                }
            }
        }
        
        if (!found) {
            // No process can finish, system is not in safe state
            return false;
        }
    }
    
    // Print safe sequence
    printf("\nSafe Sequence: ");
    for (int i = 0; i < n_processes; i++) {
        printf("P%d ", safe_sequence[i]);
        if (i < n_processes - 1) {
            printf("-> ");
        }
    }
    printf("\n");
    
    return true;
}

// Try to allocate resources to a process
bool request_resources(int process_id, int request[], int available[], int max[][MAX_RESOURCES], 
                      int allocation[][MAX_RESOURCES], int need[][MAX_RESOURCES], 
                      int n_processes, int n_resources) {
    // Check if request is valid
    for (int i = 0; i < n_resources; i++) {
        if (request[i] > need[process_id][i]) {
            printf("Error: Process has requested more resources than its maximum claim.\n");
            return false;
        }
        
        if (request[i] > available[i]) {
            printf("Error: Resources are not available. Process must wait.\n");
            return false;
        }
    }
    
    // Try resource allocation
    for (int i = 0; i < n_resources; i++) {
        available[i] -= request[i];
        allocation[process_id][i] += request[i];
        need[process_id][i] -= request[i];
    }
    
    // Check if resulting state is safe
    if (is_safe_state(available, max, allocation, need, n_processes, n_resources)) {
        return true;
    }
    
    // If not safe, rollback changes
    for (int i = 0; i < n_resources; i++) {
        available[i] += request[i];
        allocation[process_id][i] -= request[i];
        need[process_id][i] += request[i];
    }
    
    return false;
}