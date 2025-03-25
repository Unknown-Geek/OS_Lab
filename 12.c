#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct {
    int pid;
    int at;
    int bt;
    int priority;
}Process;


void sort(Process process[50],int n) {
    Process temp;
    for(int i=0 ; i<n-1 ; i++) {
        for(int j=0 ;j<n-i-1 ; j++) {
            if(process[j].at > process[j+1].at) {
                temp = process[j];
                process[j] = process[j+1];
                process[j+1] = temp;
            }
        }
    }
}

void FCFS(Process process[50],int n) {
    int waiting_time = 0;
    int ct[n];
    int time = 0;
    int total_wait = 0;
    for(int i=0 ; i<n ; i++) {
        if(time < process[i].at)
            time = process[i].at;
        time += process[i].bt;
        waiting_time = time - process[i].at - process[i].bt;
        total_wait += waiting_time;
        
    }
    printf("FCFS - Total Waiting Time: %d, Average Waiting Time: %.2f\n", total_wait, (float)total_wait/n);
}

void SJF(Process process[50],int n) {
    int waiting_time = 0;
    int total_wait = 0;
    int time = 0;
    int completion_count = 0;
    int min_burst;
    int selected;
    int completed[n];
    for(int i = 0; i < n; i++) {
        completed[i] = 0;
    }

    while(completion_count < n) {
        min_burst = 999;
        selected = -1;
        for(int i=0 ; i<n ; i++) {
            if(time == 0 ? (process[i].bt < min_burst && completed[i] == 0) : (process[i].bt < min_burst && completed[i] == 0 && process[i].at <= time)) {
                selected = i;
                min_burst = process[i].bt;
            }
        }
        if (selected == -1) {
            time++;
            continue;
        }
        if(time == 0)
            time = process[selected].at;

        time += process[selected].bt;
        waiting_time = time - process[selected].at - process[selected].bt;
        total_wait += waiting_time;
        
        completed[selected] = 1;
        completion_count++;
    }
    printf("SJF - Total Waiting Time: %d, Average Waiting Time: %.2f\n", total_wait, (float)total_wait/n);
}

void SRTF(Process process[50],int n) {
    int waiting_time = 0;
    int total_wait  = 0;
    int time = 0;
    int selected;
    int completed_count = 0,found;
    int min_rt;
    int remaining_time[n],completed[n];

    for(int i=0 ; i<n ; i++) {
        completed[i] = 0;
        remaining_time[i] = process[i].bt;
    }

    while (completed_count < n) {
        min_rt = 999;
        found = 0;
        for(int i=0 ; i<n ; i++) {
            if(completed[i] == 0 && process[i].at <= time && remaining_time[i] <= min_rt && remaining_time[i] > 0) {
                found = 1;
                selected = i;
                min_rt = remaining_time[i];
            }
        }

        if(!found){
            time++;
            continue;
        }

        
        remaining_time[selected]--;
        time ++;

        if(remaining_time[selected] == 0) {
            completed[selected] = 1;
            completed_count++;
            waiting_time = time - process[selected].at - process[selected].bt;
            total_wait += waiting_time;
        }
        
    }
    printf("SRTF - Total Waiting Time: %d, Average Waiting Time: %.2f\n", total_wait, (float)total_wait/n);
}

void Priority(Process process[50],int n) {
    int time = 0;
    int waiting_time,total_wait = 0;
    int selected = -1;
    int completed_count = 0;
    int priority = -1;
    int completed[n];
    for(int i = 0; i < n; i++) {
        completed[i] = 0;
    }

    while(completed_count < n) {
        priority = -1;
        for(int i=0 ; i<n ; i++ ) {
            if(completed[i] == 0 && time >= process[i].at && process[i].priority >= priority) {
                priority = process[i].priority;
                selected = i;
            }
        }
        if(selected == -1) {
            time++;
            continue;
        }
        time += process[selected].bt;
        waiting_time = time - process[selected].at - process[selected].bt;
        total_wait += waiting_time;
        completed[selected] = 1;
        completed_count++;
    }    
    printf("Priority - Total Waiting Time: %d, Average Waiting Time: %.2f\n", total_wait, (float)total_wait/n);
}

void RR(Process process[50], int n, int quantum) {
    int time = 0;
    int waiting_time = 0;
    int total_wait = 0;
    int selected;
    int completed_count = 0;

    int completed[n];
    for(int i = 0; i < n; i++) {
        completed[i] = 0;
    }

    int remaining_time[n];
    for(int i = 0; i < n; i++) {
        remaining_time[i] = process[i].bt;
    }

    while(completed_count < n) {
        int found = 0;
        for(int i = 0; i < n; i++) {
            if(completed[i] == 0 && process[i].at <= time) {
                found = 1;
                selected = i;
                if(remaining_time[i] <= quantum) {
                    time += remaining_time[i];
                    waiting_time = time - process[selected].at - process[selected].bt;
                    total_wait += waiting_time;
                    
                    completed[i] = 1;
                    completed_count++;
                    remaining_time[i] = 0;
                }
                else {
                    remaining_time[i] -= quantum;
                    time += quantum;
                }
            }
        }
        if(!found)
            time++;
    }
    
    printf("Round Robin (Quantum=%d) - Total Waiting Time: %d, Average Waiting Time: %.2f\n", 
           quantum, total_wait, (float)total_wait/n);
}

int main() {
    Process process[50];
    int n, ch;
    int quantum = 1;
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    printf("Enter PID, arrival time, burst time, and priority: \n");
    for (int i = 0; i < n; i++) {
        scanf("%d %d %d %d", &process[i].pid, &process[i].at, &process[i].bt, &process[i].priority);
    }
    sort(process, n);
    printf("\nMENU\n1.FCFS\n2.SJF\n3.SRTF\n4.Priority\n5.RR\n6.Algorithm with minimum average waiting time\n7.Exit\n");
    while (1) {
        printf("\n");
        printf("Enter your choice: ");
        scanf("%d", &ch);
        switch (ch) {
            case 1: 
                FCFS(process, n); 
                break;
            case 2: 
                SJF(process, n); 
                break;
            case 3: 
                SRTF(process, n); 
                break;
            case 4: 
                Priority(process, n); 
                break;
            case 5: 
                RR(process, n, quantum); 
                break;
            case 6: 
                // printf("Algorithm with minimum average waiting time is/are :\n");
                // for(int i =0 ; i<n ; i++) {
                //     if(values[i] == minimum_wait)
                //         printf("%s with value %.2f\n",algorithms[i],minimum_wait);
                // }
                // break;
            case 7:
                exit(0);
            default: 
                printf("Wrong Choice\n");
        }
    }
}