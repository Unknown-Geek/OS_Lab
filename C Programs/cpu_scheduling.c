#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

// Structure to represent a process
typedef struct {
    int id;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int response_time;
    int priority;
    bool executed;
} Process;

// Function prototypes
void fcfs(Process processes[], int n);
void sjf(Process processes[], int n);
void srtf(Process processes[], int n);
void priority_scheduling(Process processes[], int n);
void round_robin(Process processes[], int n, int quantum);
void calculate_times(Process processes[], int n);
void display_results(Process processes[], int n, const char* algorithm);

int main() {
    int n, quantum;
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    
    Process processes[n];
    
    // Input process details
    for(int i = 0; i < n; i++) {
        processes[i].id = i + 1;
        printf("Enter arrival time for Process %d: ", i+1);
        scanf("%d", &processes[i].arrival_time);
        printf("Enter burst time for Process %d: ", i+1);
        scanf("%d", &processes[i].burst_time);
        printf("Enter priority for Process %d: ", i+1);
        scanf("%d", &processes[i].priority);
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].executed = false;
    }
    
    printf("Enter time quantum for Round Robin: ");
    scanf("%d", &quantum);
    
    // Create copies of the original process array for each algorithm
    Process fcfs_p[n], sjf_p[n], srtf_p[n], priority_p[n], rr_p[n];
    for(int i = 0; i < n; i++) {
        fcfs_p[i] = processes[i];
        sjf_p[i] = processes[i];
        srtf_p[i] = processes[i];
        priority_p[i] = processes[i];
        rr_p[i] = processes[i];
    }
    
    // Run scheduling algorithms
    fcfs(fcfs_p, n);
    sjf(sjf_p, n);
    srtf(srtf_p, n);
    priority_scheduling(priority_p, n);
    round_robin(rr_p, n, quantum);
    
    return 0;
}

// First Come First Serve scheduling
void fcfs(Process processes[], int n) {
    int current_time = 0;
    
    // Sort processes by arrival time
    for(int i = 0; i < n-1; i++) {
        for(int j = 0; j < n-i-1; j++) {
            if(processes[j].arrival_time > processes[j+1].arrival_time) {
                Process temp = processes[j];
                processes[j] = processes[j+1];
                processes[j+1] = temp;
            }
        }
    }
    
    for(int i = 0; i < n; i++) {
        if(current_time < processes[i].arrival_time) {
            current_time = processes[i].arrival_time;
        }
        
        processes[i].response_time = current_time - processes[i].arrival_time;
        processes[i].completion_time = current_time + processes[i].burst_time;
        current_time = processes[i].completion_time;
    }
    
    calculate_times(processes, n);
    display_results(processes, n, "FCFS");
}

// Shortest Job First scheduling
void sjf(Process processes[], int n) {
    int completed = 0;
    int current_time = 0;
    
    // Process all jobs in order of their burst time
    while(completed < n) {
        int shortest_idx = -1;
        int shortest_burst = INT_MAX;
        
        for(int i = 0; i < n; i++) {
            if(!processes[i].executed && 
               processes[i].arrival_time <= current_time && 
               processes[i].burst_time < shortest_burst) {
                shortest_burst = processes[i].burst_time;
                shortest_idx = i;
            }
        }
        
        if(shortest_idx == -1) {
            // No process available, advance time
            current_time++;
        } else {
            // Execute the process
            processes[shortest_idx].response_time = current_time - processes[shortest_idx].arrival_time;
            processes[shortest_idx].completion_time = current_time + processes[shortest_idx].burst_time;
            current_time = processes[shortest_idx].completion_time;
            processes[shortest_idx].executed = true;
            completed++;
        }
    }
    
    calculate_times(processes, n);
    display_results(processes, n, "SJF");
}

// Shortest Remaining Time First scheduling
void srtf(Process processes[], int n) {
    int completed = 0;
    int current_time = 0;
    int shortest_idx;
    
    while(completed < n) {
        shortest_idx = -1;
        int shortest_remaining = INT_MAX;
        
        for(int i = 0; i < n; i++) {
            if(processes[i].arrival_time <= current_time && 
               processes[i].remaining_time > 0 && 
               processes[i].remaining_time < shortest_remaining) {
                shortest_remaining = processes[i].remaining_time;
                shortest_idx = i;
            }
        }
        
        if(shortest_idx == -1) {
            current_time++;
        } else {
            // Track response time on first execution
            if(processes[shortest_idx].remaining_time == processes[shortest_idx].burst_time) {
                processes[shortest_idx].response_time = current_time - processes[shortest_idx].arrival_time;
            }
            
            processes[shortest_idx].remaining_time--;
            current_time++;
            
            if(processes[shortest_idx].remaining_time == 0) {
                completed++;
                processes[shortest_idx].completion_time = current_time;
            }
        }
    }
    
    calculate_times(processes, n);
    display_results(processes, n, "SRTF");
}

// Priority Scheduling
void priority_scheduling(Process processes[], int n) {
    int completed = 0;
    int current_time = 0;
    
    while(completed < n) {
        int highest_priority_idx = -1;
        int highest_priority = INT_MAX;
        
        for(int i = 0; i < n; i++) {
            if(!processes[i].executed && 
               processes[i].arrival_time <= current_time && 
               processes[i].priority < highest_priority) {
                highest_priority = processes[i].priority;
                highest_priority_idx = i;
            }
        }
        
        if(highest_priority_idx == -1) {
            current_time++;
        } else {
            processes[highest_priority_idx].response_time = current_time - processes[highest_priority_idx].arrival_time;
            processes[highest_priority_idx].completion_time = current_time + processes[highest_priority_idx].burst_time;
            current_time = processes[highest_priority_idx].completion_time;
            processes[highest_priority_idx].executed = true;
            completed++;
        }
    }
    
    calculate_times(processes, n);
    display_results(processes, n, "Priority");
}

// Round Robin Scheduling
void round_robin(Process processes[], int n, int quantum) {
    int completed = 0;
    int current_time = 0;
    bool responded[n];
    
    for(int i = 0; i < n; i++) {
        responded[i] = false;
    }
    
    // Sort processes by arrival time
    for(int i = 0; i < n-1; i++) {
        for(int j = 0; j < n-i-1; j++) {
            if(processes[j].arrival_time > processes[j+1].arrival_time) {
                Process temp = processes[j];
                processes[j] = processes[j+1];
                processes[j+1] = temp;
            }
        }
    }
    
    int queue[100], front = 0, rear = 0;
    
    // Add first process to queue
    queue[rear++] = 0;
    
    while(completed < n) {
        if(front == rear) {
            // Queue is empty, find next process by arrival time
            int next_arrival = INT_MAX;
            int next_idx = -1;
            
            for(int i = 0; i < n; i++) {
                if(processes[i].remaining_time > 0 && processes[i].arrival_time < next_arrival) {
                    next_arrival = processes[i].arrival_time;
                    next_idx = i;
                }
            }
            
            if(next_idx != -1) {
                current_time = next_arrival;
                queue[rear++] = next_idx;
            }
        }
        
        int idx = queue[front++];
        
        // Record response time
        if(!responded[idx]) {
            processes[idx].response_time = current_time - processes[idx].arrival_time;
            responded[idx] = true;
        }
        
        // Execute for quantum time or until completion
        if(processes[idx].remaining_time <= quantum) {
            current_time += processes[idx].remaining_time;
            processes[idx].remaining_time = 0;
            processes[idx].completion_time = current_time;
            completed++;
        } else {
            current_time += quantum;
            processes[idx].remaining_time -= quantum;
            
            // Add newly arrived processes to queue
            for(int i = 0; i < n; i++) {
                if(processes[i].remaining_time > 0 && 
                   processes[i].arrival_time <= current_time && 
                   i != idx) {
                    // Check if process is already in queue
                    bool in_queue = false;
                    for(int j = front; j < rear; j++) {
                        if(queue[j] == i) {
                            in_queue = true;
                            break;
                        }
                    }
                    
                    if(!in_queue) {
                        queue[rear++] = i;
                    }
                }
            }
            
            // Add current process back to queue
            queue[rear++] = idx;
        }
    }
    
    calculate_times(processes, n);
    display_results(processes, n, "Round Robin");
}

// Calculate turnaround and waiting times
void calculate_times(Process processes[], int n) {
    for(int i = 0; i < n; i++) {
        processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time;
        processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;
    }
}

// Display results of scheduling algorithm
void display_results(Process processes[], int n, const char* algorithm) {
    printf("\n%s Scheduling Results:\n", algorithm);
    printf("=====================================================\n");
    printf("Process\tArrival\tBurst\tPriority\tCompletion\tTurnaround\tWaiting\tResponse\n");
    
    float avg_turnaround = 0, avg_waiting = 0, avg_response = 0;
    
    for(int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t\t%d\t\t%d\t\t%d\t%d\n", 
               processes[i].id, processes[i].arrival_time, processes[i].burst_time, 
               processes[i].priority, processes[i].completion_time, 
               processes[i].turnaround_time, processes[i].waiting_time, 
               processes[i].response_time);
        
        avg_turnaround += processes[i].turnaround_time;
        avg_waiting += processes[i].waiting_time;
        avg_response += processes[i].response_time;
    }
    
    avg_turnaround /= n;
    avg_waiting /= n;
    avg_response /= n;
    
    printf("\nAverage Turnaround Time: %.2f\n", avg_turnaround);
    printf("Average Waiting Time: %.2f\n", avg_waiting);
    printf("Average Response Time: %.2f\n", avg_response);
    printf("=====================================================\n");
}