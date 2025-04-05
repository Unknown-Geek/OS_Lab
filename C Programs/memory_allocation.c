#include <stdio.h>
#include <stdlib.h>

#define MEM_SIZE 4000
#define LEN 6

int mem[LEN] = {310, 90, 350, 185, 200, 850};
int start[LEN] = {0, 310, 400, 750, 1850, 2200};

void first_fit(int req) {
    int found = -1;
    for (int i = 0; i < LEN; i++) {
        if (mem[i] >= req) {
            found = i;
            mem[i] -= req;
            start[i] = start[i] + req;
            break;
        }
    }
    if (found == -1) {
        printf("Required memory block not found\n");
    }
}

void best_fit(int req) {
    int found = -1;
    int min = MEM_SIZE * MEM_SIZE;
    for (int i = 0; i < LEN; i++) {
        if (mem[i] >= req && (mem[i] - req) < min) {
            min = mem[i] - req;
            found = i;
        }
    }
    if (found == -1) {
        printf("Required memory block not found\n");
        return;
    }

    mem[found] -= req;
    start[found] = start[found] + req;
}

void worst_fit(int req) {
    int found = -1;
    int max = -1;

    for (int i = 0; i < LEN; i++) {
        if (mem[i] >= req && mem[i] > max) {
            max = mem[i];
            found = i;
        }
    }

    if (found == -1) {
        printf("Required memory block not found\n");
        return;
    }

    mem[found] -= req;
    start[found] = start[found] + req;
}

void display_memory() {
    printf("\nMemory Blocks Status:\n");
    printf("---------------------\n");
    printf("Block\tSize\tStart Address\n");
    for (int i = 0; i < LEN; i++) {
        printf("%d\t%d\t%d\n", i, mem[i], start[i]);
    }
    printf("\n");
}

int main() {
    int choice, req;

    while (1) {
        printf("\nMemory Allocation Algorithms\n");
        printf("1. First Fit\n");
        printf("2. Best Fit\n");
        printf("3. Worst Fit\n");
        printf("4. Display Memory Status\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Enter memory request size: ");
            scanf("%d", &req);
            first_fit(req);
            display_memory();
            break;

        case 2:
            printf("Enter memory request size: ");
            scanf("%d", &req);
            best_fit(req);
            display_memory();
            break;

        case 3:
            printf("Enter memory request size: ");
            scanf("%d", &req);
            worst_fit(req);
            display_memory();
            break;

        case 4:
            display_memory();
            break;

        case 5:
            printf("Exiting program.\n");
            exit(0);

        default:
            printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}