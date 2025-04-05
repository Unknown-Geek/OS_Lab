#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_BLOCKS 50
#define MAX_PROCESSES 50

// Structure to represent a memory block
typedef struct {
    int id;
    int size;
    bool allocated;
    int process_id;  // ID of the process that allocated this block (-1 if free)
} MemoryBlock;

// Structure to represent a process
typedef struct {
    int id;
    int size;
    int block_id;    // ID of the block where process is allocated (-1 if not allocated)
} Process;

// Function prototypes
void initialize_memory(MemoryBlock blocks[], int n);
void display_memory_status(MemoryBlock blocks[], int n);
void first_fit(MemoryBlock blocks[], int n_blocks, Process processes[], int n_processes);
void best_fit(MemoryBlock blocks[], int n_blocks, Process processes[], int n_processes);
void worst_fit(MemoryBlock blocks[], int n_blocks, Process processes[], int n_processes);
void reset_memory(MemoryBlock blocks[], int n_blocks);
void reset_processes(Process processes[], int n_processes);

int main() {
    int n_blocks, n_processes;
    
    printf("Enter the number of memory blocks: ");
    scanf("%d", &n_blocks);
    
    MemoryBlock blocks[MAX_BLOCKS];
    
    printf("Enter the size of each memory block:\n");
    for (int i = 0; i < n_blocks; i++) {
        printf("Block %d: ", i + 1);
        scanf("%d", &blocks[i].size);
        blocks[i].id = i + 1;
        blocks[i].allocated = false;
        blocks[i].process_id = -1;
    }
    
    printf("Enter the number of processes: ");
    scanf("%d", &n_processes);
    
    Process processes[MAX_PROCESSES];
    
    printf("Enter the size of each process:\n");
    for (int i = 0; i < n_processes; i++) {
        printf("Process %d: ", i + 1);
        scanf("%d", &processes[i].size);
        processes[i].id = i + 1;
        processes[i].block_id = -1;
    }
    
    printf("\n\n--- First Fit Memory Allocation ---\n");
    first_fit(blocks, n_blocks, processes, n_processes);
    
    // Reset the memory and processes for next algorithm
    reset_memory(blocks, n_blocks);
    reset_processes(processes, n_processes);
    
    printf("\n\n--- Best Fit Memory Allocation ---\n");
    best_fit(blocks, n_blocks, processes, n_processes);
    
    // Reset the memory and processes for next algorithm
    reset_memory(blocks, n_blocks);
    reset_processes(processes, n_processes);
    
    printf("\n\n--- Worst Fit Memory Allocation ---\n");
    worst_fit(blocks, n_blocks, processes, n_processes);
    
    return 0;
}

// Initialize memory blocks
void initialize_memory(MemoryBlock blocks[], int n) {
    for (int i = 0; i < n; i++) {
        blocks[i].allocated = false;
        blocks[i].process_id = -1;
    }
}

// Display current memory status
void display_memory_status(MemoryBlock blocks[], int n) {
    printf("\nCurrent Memory Status:\n");
    printf("Block ID\tSize\tStatus\t\tProcess ID\n");
    printf("--------------------------------------------------\n");
    
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t", blocks[i].id, blocks[i].size);
        
        if (blocks[i].allocated) {
            printf("Allocated\t%d\n", blocks[i].process_id);
        } else {
            printf("Free\t\t-\n");
        }
    }
}

// First Fit memory allocation
void first_fit(MemoryBlock blocks[], int n_blocks, Process processes[], int n_processes) {
    int allocated_count = 0;
    
    printf("Initial Memory Status:\n");
    display_memory_status(blocks, n_blocks);
    
    for (int i = 0; i < n_processes; i++) {
        bool allocated = false;
        
        for (int j = 0; j < n_blocks; j++) {
            if (!blocks[j].allocated && blocks[j].size >= processes[i].size) {
                // Allocate this block to the process
                blocks[j].allocated = true;
                blocks[j].process_id = processes[i].id;
                processes[i].block_id = blocks[j].id;
                allocated = true;
                allocated_count++;
                
                printf("\nProcess %d (Size: %d) allocated to Block %d (Size: %d, Remaining: %d)\n",
                      processes[i].id, processes[i].size, blocks[j].id, blocks[j].size, 
                      blocks[j].size - processes[i].size);
                break;
            }
        }
        
        if (!allocated) {
            printf("\nProcess %d (Size: %d) cannot be allocated - No suitable block found\n", 
                  processes[i].id, processes[i].size);
        }
    }
    
    printf("\nFirst Fit Allocation Complete. %d/%d processes allocated.\n", 
          allocated_count, n_processes);
    display_memory_status(blocks, n_blocks);
    
    // Calculate and display memory utilization
    int total_memory = 0;
    int used_memory = 0;
    int wasted_memory = 0;
    
    for (int i = 0; i < n_blocks; i++) {
        total_memory += blocks[i].size;
        
        if (blocks[i].allocated) {
            used_memory += processes[blocks[i].process_id - 1].size;
            wasted_memory += blocks[i].size - processes[blocks[i].process_id - 1].size;
        }
    }
    
    printf("\nMemory Utilization:\n");
    printf("Total Memory: %d\n", total_memory);
    printf("Used Memory: %d (%.2f%%)\n", used_memory, ((float)used_memory / total_memory) * 100);
    printf("Wasted Memory (Internal Fragmentation): %d (%.2f%%)\n", 
          wasted_memory, ((float)wasted_memory / total_memory) * 100);
}

// Best Fit memory allocation
void best_fit(MemoryBlock blocks[], int n_blocks, Process processes[], int n_processes) {
    int allocated_count = 0;
    
    printf("Initial Memory Status:\n");
    display_memory_status(blocks, n_blocks);
    
    for (int i = 0; i < n_processes; i++) {
        int best_block_index = -1;
        int minimum_block_size = INT_MAX;
        
        // Find the smallest suitable block
        for (int j = 0; j < n_blocks; j++) {
            if (!blocks[j].allocated && blocks[j].size >= processes[i].size) {
                if (blocks[j].size < minimum_block_size) {
                    minimum_block_size = blocks[j].size;
                    best_block_index = j;
                }
            }
        }
        
        if (best_block_index != -1) {
            // Allocate the best fit block to the process
            blocks[best_block_index].allocated = true;
            blocks[best_block_index].process_id = processes[i].id;
            processes[i].block_id = blocks[best_block_index].id;
            allocated_count++;
            
            printf("\nProcess %d (Size: %d) allocated to Block %d (Size: %d, Remaining: %d)\n",
                  processes[i].id, processes[i].size, blocks[best_block_index].id, 
                  blocks[best_block_index].size, 
                  blocks[best_block_index].size - processes[i].size);
        } else {
            printf("\nProcess %d (Size: %d) cannot be allocated - No suitable block found\n", 
                  processes[i].id, processes[i].size);
        }
    }
    
    printf("\nBest Fit Allocation Complete. %d/%d processes allocated.\n", 
          allocated_count, n_processes);
    display_memory_status(blocks, n_blocks);
    
    // Calculate and display memory utilization
    int total_memory = 0;
    int used_memory = 0;
    int wasted_memory = 0;
    
    for (int i = 0; i < n_blocks; i++) {
        total_memory += blocks[i].size;
        
        if (blocks[i].allocated) {
            used_memory += processes[blocks[i].process_id - 1].size;
            wasted_memory += blocks[i].size - processes[blocks[i].process_id - 1].size;
        }
    }
    
    printf("\nMemory Utilization:\n");
    printf("Total Memory: %d\n", total_memory);
    printf("Used Memory: %d (%.2f%%)\n", used_memory, ((float)used_memory / total_memory) * 100);
    printf("Wasted Memory (Internal Fragmentation): %d (%.2f%%)\n", 
          wasted_memory, ((float)wasted_memory / total_memory) * 100);
}

// Worst Fit memory allocation
void worst_fit(MemoryBlock blocks[], int n_blocks, Process processes[], int n_processes) {
    int allocated_count = 0;
    
    printf("Initial Memory Status:\n");
    display_memory_status(blocks, n_blocks);
    
    for (int i = 0; i < n_processes; i++) {
        int worst_block_index = -1;
        int maximum_block_size = -1;
        
        // Find the largest suitable block
        for (int j = 0; j < n_blocks; j++) {
            if (!blocks[j].allocated && blocks[j].size >= processes[i].size) {
                if (blocks[j].size > maximum_block_size) {
                    maximum_block_size = blocks[j].size;
                    worst_block_index = j;
                }
            }
        }
        
        if (worst_block_index != -1) {
            // Allocate the worst fit block to the process
            blocks[worst_block_index].allocated = true;
            blocks[worst_block_index].process_id = processes[i].id;
            processes[i].block_id = blocks[worst_block_index].id;
            allocated_count++;
            
            printf("\nProcess %d (Size: %d) allocated to Block %d (Size: %d, Remaining: %d)\n",
                  processes[i].id, processes[i].size, blocks[worst_block_index].id, 
                  blocks[worst_block_index].size, 
                  blocks[worst_block_index].size - processes[i].size);
        } else {
            printf("\nProcess %d (Size: %d) cannot be allocated - No suitable block found\n", 
                  processes[i].id, processes[i].size);
        }
    }
    
    printf("\nWorst Fit Allocation Complete. %d/%d processes allocated.\n", 
          allocated_count, n_processes);
    display_memory_status(blocks, n_blocks);
    
    // Calculate and display memory utilization
    int total_memory = 0;
    int used_memory = 0;
    int wasted_memory = 0;
    
    for (int i = 0; i < n_blocks; i++) {
        total_memory += blocks[i].size;
        
        if (blocks[i].allocated) {
            used_memory += processes[blocks[i].process_id - 1].size;
            wasted_memory += blocks[i].size - processes[blocks[i].process_id - 1].size;
        }
    }
    
    printf("\nMemory Utilization:\n");
    printf("Total Memory: %d\n", total_memory);
    printf("Used Memory: %d (%.2f%%)\n", used_memory, ((float)used_memory / total_memory) * 100);
    printf("Wasted Memory (Internal Fragmentation): %d (%.2f%%)\n", 
          wasted_memory, ((float)wasted_memory / total_memory) * 100);
}

// Reset memory blocks to unallocated state
void reset_memory(MemoryBlock blocks[], int n_blocks) {
    for (int i = 0; i < n_blocks; i++) {
        blocks[i].allocated = false;
        blocks[i].process_id = -1;
    }
}

// Reset processes to unallocated state
void reset_processes(Process processes[], int n_processes) {
    for (int i = 0; i < n_processes; i++) {
        processes[i].block_id = -1;
    }
}