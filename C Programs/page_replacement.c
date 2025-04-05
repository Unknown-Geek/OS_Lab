#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FRAMES 4
#define LEN 20

int ref[LEN];

void ref_string() {
    for (int i = 0; i < LEN; i++) {
        ref[i] = rand() % 10;
    }
}

int page_check(int frames[FRAMES], int key) {
    for (int i = 0; i < FRAMES; i++) {
        if (frames[i] == key)
            return 1;
    }
    return 0;
}

int fifo(int ref[LEN]) {
    int faults = 0;
    int index = 0;
    int frames[FRAMES];

    for (int i = 0; i < FRAMES; i++) {
        frames[i] = -1;
    }

    for (int i = 0; i < LEN; i++) {
        if (!page_check(frames, ref[i])) {
            frames[index] = ref[i];
            index = (index + 1) % FRAMES;
            faults++;
        }
    }

    return faults;
}

int lru(int ref[LEN]) {
    int faults = 0;
    int rec[FRAMES];
    int frames[FRAMES];
    int count = 0;

    for (int i = 0; i < FRAMES; i++) {
        frames[i] = -1;
        rec[i] = 0;
    }

    for (int i = 0; i < LEN; i++) {
        if (!page_check(frames, ref[i])) {
            int min = -1;
            for (int j = 0; j < FRAMES; j++) {
                if (frames[j] == -1) {
                    min = j;
                    break;
                }
            }
            
            if (min == -1) {
                min = 0;
                for (int j = 1; j < FRAMES; j++) {
                    if (rec[j] < rec[min]) {
                        min = j;
                    }
                }
            }

            frames[min] = ref[i];
            rec[min] = i + 1; // Use current time (i) as timestamp
            faults++;
        }
        else {
            // Update timestamp for the accessed page
            for (int j = 0; j < FRAMES; j++) {
                if (frames[j] == ref[i]) {
                    rec[j] = i + 1; // Update timestamp
                    break;
                }
            }
        }
    }
    return faults;
}

int lfu(int ref[LEN]) {
    int faults = 0;
    int frames[FRAMES];
    int freq[FRAMES]; // Track frequency for each frame, not for each possible page

    for (int i = 0; i < FRAMES; i++) {
        frames[i] = -1;
        freq[i] = 0;
    }

    for (int i = 0; i < LEN; i++) {
        if (!page_check(frames, ref[i])) {
            // Find the frame with least frequency or initialize empty frame
            int lf_index = 0;
            for (int j = 1; j < FRAMES; j++) {
                if (frames[j] == -1) { // Empty frame has priority
                    lf_index = j;
                    break;
                }
                if (freq[j] < freq[lf_index]) {
                    lf_index = j;
                }
            }
            frames[lf_index] = ref[i];
            freq[lf_index] = 1; 
            faults++;
        }
        else {
            for (int j = 0; j < FRAMES; j++) {
                if (frames[j] == ref[i]) {
                    freq[j]++;
                    break;
                }
            }
        }
    }
    return faults;
}

int main() {
    srand(time(NULL));
    ref_string();

    printf("Reference string: ");
    for (int i = 0; i < LEN; i++) {
        printf("%d ", ref[i]);
    }
    printf("\n");

    int fifo_faults = fifo(ref);
    int lru_faults = lru(ref);
    int lfu_faults = lfu(ref);

    printf("Page Faults using FIFO: %d\n", fifo_faults);
    printf("Page Faults using LRU: %d\n", lru_faults);
    printf("Page Faults using LFU: %d\n", lfu_faults);

    return 0;
}