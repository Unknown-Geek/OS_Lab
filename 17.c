#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DISK_SIZE 5000
#define LEN 10

void printRequests(int requests[], int n) { 
    printf("Generated requests: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", requests[i]);
    }
    printf("\n");
}

int fcfs(int requests[], int n, int head) {
    int total_movement = 0;
    
    printf("\nFCFS Order: %d ", head);
    for (int i = 0; i < n; i++) {
        total_movement += abs(requests[i] - head);
        head = requests[i];
        printf("-> %d ", head);
    }
    
    return total_movement;
}

int scan(int requests[], int n, int head, int direction, int last_req) {
    int total_movement = 0;
    int temp[LEN + 2];
    int size = 0;
    int index;
    int curr_head = head;
    
    for (int i = 0; i < n; i++) {
        temp[size++] = requests[i];
    }
    
    if (direction == 1) {
        temp[size++] = DISK_SIZE - 1;
    } else {
        temp[size++] = 0;
    }
    
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (temp[j] > temp[j + 1]) {
                int t = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = t;
            }
        }
    }
    
    for (index = 0; index < size; index++) {
        if (temp[index] >= head) {
            break;
        }
    }
    
    printf("\nSCAN Order: %d ", head);
    
    if (direction == 1) {
        for (int i = index; i < size; i++) {
            if (temp[i] != head) {
                total_movement += abs(temp[i] - curr_head);
                curr_head = temp[i];
                printf("-> %d ", curr_head);
            }
        }
        
        for (int i = index - 1; i >= 0; i--) {
            total_movement += abs(temp[i] - curr_head);
            curr_head = temp[i];
            printf("-> %d ", curr_head);
        }
    }
    else {
        for (int i = index - 1; i >= 0; i--) {
            total_movement += abs(temp[i] - curr_head);
            curr_head = temp[i];
            printf("-> %d ", curr_head);
        }
        
        for (int i = index; i < size; i++) {
            if (temp[i] != head) {
                total_movement += abs(temp[i] - curr_head);
                curr_head = temp[i];
                printf("-> %d ", curr_head);
            }
        }
    }
    
    return total_movement;
}

int look(int requests[], int n, int head, int direction) {
    int total_movement = 0;
    int temp[LEN];
    int size = 0;
    int index;
    int curr_head = head;
    
    for (int i = 0; i < n; i++) {
        temp[size++] = requests[i];
    }
    
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (temp[j] > temp[j + 1]) {
                int t = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = t;
            }
        }
    }
    
    for (index = 0; index < size; index++) {
        if (temp[index] >= head) {
            break;
        }
    }
    
    printf("\nLOOK Order: %d ", head);
    
    if (direction == 1) {
        for (int i = index; i < size; i++) {
            total_movement += abs(temp[i] - curr_head);
            curr_head = temp[i];
            printf("-> %d ", curr_head);
        }
        
        for (int i = index - 1; i >= 0; i--) {
            total_movement += abs(temp[i] - curr_head);
            curr_head = temp[i];
            printf("-> %d ", curr_head);
        }
    }
    else {
        for (int i = index - 1; i >= 0; i--) {
            total_movement += abs(temp[i] - curr_head);
            curr_head = temp[i];
            printf("-> %d ", curr_head);
        }
        
        for (int i = index; i < size; i++) {
            total_movement += abs(temp[i] - curr_head);
            curr_head = temp[i];
            printf("-> %d ", curr_head);
        }
    }
    
    return total_movement;
}

int cscan(int requests[], int n, int head) {
    int total_movement = 0;
    int temp[LEN + 2];
    int size = 0;
    int index;
    int curr_head = head;
    
    for (int i = 0; i < n; i++) {
        temp[size++] = requests[i];
    }
    
    temp[size++] = 0;
    temp[size++] = DISK_SIZE - 1;
    
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (temp[j] > temp[j + 1]) {
                int t = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = t;
            }
        }
    }
    
    for (index = 0; index < size; index++) {
        if (temp[index] >= head) {
            break;
        }
    }
    
    printf("\nC-SCAN Order: %d ", head);
    
    for (int i = index; i < size; i++) {
        if (temp[i] != head) {
            total_movement += abs(temp[i] - curr_head);
            curr_head = temp[i];
            printf("-> %d ", curr_head);
        }
    }
    
    total_movement += abs(0 - curr_head);
    curr_head = 0;
    printf("-> %d ", curr_head);
    
    for (int i = 1; i < index; i++) {
        total_movement += abs(temp[i] - curr_head);
        curr_head = temp[i];
        printf("-> %d ", curr_head);
    }
    
    return total_movement;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <initial_head_position> <last_request>\n", argv[0]);
        return 1;
    }

    int head = atoi(argv[1]);
    int last_request = atoi(argv[2]);
    int requests[LEN];
    int total_movement;
    
    if (head < 0 || head >= DISK_SIZE) {
        printf("Error: Initial head position must be between 0 and %d\n", DISK_SIZE - 1);
        return 1;
    }
    
    srand(time(NULL));
    
    for (int i = 0; i < LEN; i++) {
        requests[i] = rand() % DISK_SIZE;
    }
    
    requests[LEN - 1] = last_request;
    
    printRequests(requests, LEN);
    
    total_movement = fcfs(requests, LEN, head);
    printf("\nFCFS Total Movement: %d\n", total_movement);
    
    total_movement = scan(requests, LEN, head, 1, last_request);
    printf("\nSCAN Total Movement: %d\n", total_movement);
    
    total_movement = look(requests, LEN, head, 1);
    printf("\nLOOK Total Movement: %d\n", total_movement);
    
    total_movement = cscan(requests, LEN, head);
    printf("\nC-SCAN Total Movement: %d\n", total_movement);
    
    return 0;
}
