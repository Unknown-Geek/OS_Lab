#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_FRAMES 10
#define MAX_PAGES 50

// Function prototypes
void fifo(int pages[], int n, int num_frames);
void lru(int pages[], int n, int num_frames);
void lfu(int pages[], int n, int num_frames);
int find_page(int frame_table[], int num_frames, int page);
int find_lru_page(int recent_counter[], int num_frames);
int find_lfu_page(int frequency[], int recent_counter[], int num_frames);

int main() {
    int n, num_frames;
    int pages[MAX_PAGES];
    
    printf("Enter number of pages: ");
    scanf("%d", &n);
    
    printf("Enter the reference string: ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &pages[i]);
    }
    
    printf("Enter number of frames: ");
    scanf("%d", &num_frames);
    
    printf("\nFIFO Page Replacement:\n");
    fifo(pages, n, num_frames);
    
    printf("\nLRU Page Replacement:\n");
    lru(pages, n, num_frames);
    
    printf("\nLFU Page Replacement:\n");
    lfu(pages, n, num_frames);
    
    return 0;
}

// First In First Out page replacement algorithm
void fifo(int pages[], int n, int num_frames) {
    int frame_table[MAX_FRAMES];
    int page_faults = 0;
    int next_frame = 0;
    
    // Initialize frame table with -1 (empty frames)
    for (int i = 0; i < num_frames; i++) {
        frame_table[i] = -1;
    }
    
    printf("Reference String: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", pages[i]);
    }
    printf("\n");
    
    for (int i = 0; i < n; i++) {
        int page = pages[i];
        
        // Check if page is already in a frame
        if (find_page(frame_table, num_frames, page) == -1) {
            // Page fault occurred
            frame_table[next_frame] = page;
            next_frame = (next_frame + 1) % num_frames;
            page_faults++;
            
            // Print frame table state
            printf("Page %d -> [", page);
            for (int j = 0; j < num_frames; j++) {
                if (frame_table[j] == -1) {
                    printf(" -");
                } else {
                    printf(" %d", frame_table[j]);
                }
            }
            printf(" ] Page fault #%d\n", page_faults);
        } else {
            // Page hit
            printf("Page %d -> [", page);
            for (int j = 0; j < num_frames; j++) {
                if (frame_table[j] == -1) {
                    printf(" -");
                } else {
                    printf(" %d", frame_table[j]);
                }
            }
            printf(" ] No page fault\n");
        }
    }
    
    printf("Total page faults: %d\n", page_faults);
    printf("Hit ratio: %.2f%%\n", ((float)(n - page_faults) / n) * 100);
}

// Least Recently Used page replacement algorithm
void lru(int pages[], int n, int num_frames) {
    int frame_table[MAX_FRAMES];
    int recent_counter[MAX_FRAMES]; // Tracks how recently each frame was used
    int page_faults = 0;
    
    // Initialize frame table with -1 (empty frames)
    for (int i = 0; i < num_frames; i++) {
        frame_table[i] = -1;
        recent_counter[i] = 0;
    }
    
    printf("Reference String: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", pages[i]);
    }
    printf("\n");
    
    for (int i = 0; i < n; i++) {
        int page = pages[i];
        int frame_index = find_page(frame_table, num_frames, page);
        
        // Increment the counter for all frames to indicate they are getting older
        for (int j = 0; j < num_frames; j++) {
            if (frame_table[j] != -1) {
                recent_counter[j]++;
            }
        }
        
        if (frame_index == -1) {
            // Page fault occurred
            // Find the least recently used page
            int lru_index = find_lru_page(recent_counter, num_frames);
            
            // Replace it with current page
            frame_table[lru_index] = page;
            recent_counter[lru_index] = 0; // Reset counter for this frame
            page_faults++;
            
            // Print frame table state
            printf("Page %d -> [", page);
            for (int j = 0; j < num_frames; j++) {
                if (frame_table[j] == -1) {
                    printf(" -");
                } else {
                    printf(" %d", frame_table[j]);
                }
            }
            printf(" ] Page fault #%d\n", page_faults);
        } else {
            // Page hit, reset counter for this frame
            recent_counter[frame_index] = 0;
            
            // Print frame table state
            printf("Page %d -> [", page);
            for (int j = 0; j < num_frames; j++) {
                if (frame_table[j] == -1) {
                    printf(" -");
                } else {
                    printf(" %d", frame_table[j]);
                }
            }
            printf(" ] No page fault\n");
        }
    }
    
    printf("Total page faults: %d\n", page_faults);
    printf("Hit ratio: %.2f%%\n", ((float)(n - page_faults) / n) * 100);
}

// Least Frequently Used page replacement algorithm
void lfu(int pages[], int n, int num_frames) {
    int frame_table[MAX_FRAMES];
    int frequency[MAX_FRAMES];    // Tracks how many times each frame is referenced
    int recent_counter[MAX_FRAMES]; // Used to break ties (LRU among equal frequencies)
    int page_faults = 0;
    
    // Initialize frame table with -1 (empty frames)
    for (int i = 0; i < num_frames; i++) {
        frame_table[i] = -1;
        frequency[i] = 0;
        recent_counter[i] = 0;
    }
    
    printf("Reference String: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", pages[i]);
    }
    printf("\n");
    
    for (int i = 0; i < n; i++) {
        int page = pages[i];
        int frame_index = find_page(frame_table, num_frames, page);
        
        // Increment the counter for all frames to indicate they are getting older
        for (int j = 0; j < num_frames; j++) {
            if (frame_table[j] != -1) {
                recent_counter[j]++;
            }
        }
        
        if (frame_index == -1) {
            // Page fault occurred
            // Check if there's an empty frame
            int empty_index = find_page(frame_table, num_frames, -1);
            
            if (empty_index != -1) {
                // Found an empty frame
                frame_table[empty_index] = page;
                frequency[empty_index] = 1;
                recent_counter[empty_index] = 0;
            } else {
                // No empty frames, find the least frequently used page
                int lfu_index = find_lfu_page(frequency, recent_counter, num_frames);
                
                // Replace it with current page
                frame_table[lfu_index] = page;
                frequency[lfu_index] = 1;
                recent_counter[lfu_index] = 0;
            }
            
            page_faults++;
            
            // Print frame table state
            printf("Page %d -> [", page);
            for (int j = 0; j < num_frames; j++) {
                if (frame_table[j] == -1) {
                    printf(" -");
                } else {
                    printf(" %d", frame_table[j]);
                }
            }
            printf(" ] Page fault #%d\n", page_faults);
        } else {
            // Page hit, increment frequency and reset recent counter
            frequency[frame_index]++;
            recent_counter[frame_index] = 0;
            
            // Print frame table state
            printf("Page %d -> [", page);
            for (int j = 0; j < num_frames; j++) {
                if (frame_table[j] == -1) {
                    printf(" -");
                } else {
                    printf(" %d", frame_table[j]);
                }
            }
            printf(" ] No page fault\n");
        }
    }
    
    printf("Total page faults: %d\n", page_faults);
    printf("Hit ratio: %.2f%%\n", ((float)(n - page_faults) / n) * 100);
}

// Find a page in the frame table, return its index or -1 if not found
int find_page(int frame_table[], int num_frames, int page) {
    for (int i = 0; i < num_frames; i++) {
        if (frame_table[i] == page) {
            return i;
        }
    }
    return -1;
}

// Find the least recently used page in the frame table
int find_lru_page(int recent_counter[], int num_frames) {
    int max_counter = -1;
    int lru_index = 0;
    
    for (int i = 0; i < num_frames; i++) {
        if (recent_counter[i] > max_counter) {
            max_counter = recent_counter[i];
            lru_index = i;
        }
    }
    
    return lru_index;
}

// Find the least frequently used page in the frame table
int find_lfu_page(int frequency[], int recent_counter[], int num_frames) {
    int min_frequency = INT_MAX;
    int lfu_indices[MAX_FRAMES];
    int lfu_count = 0;
    
    // Find all pages with minimum frequency
    for (int i = 0; i < num_frames; i++) {
        if (frequency[i] < min_frequency) {
            min_frequency = frequency[i];
            lfu_count = 0;
            lfu_indices[lfu_count++] = i;
        } else if (frequency[i] == min_frequency) {
            lfu_indices[lfu_count++] = i;
        }
    }
    
    // If multiple pages have same frequency, use LRU to break ties
    if (lfu_count > 1) {
        int max_recent = -1;
        int lru_index = lfu_indices[0];
        
        for (int i = 0; i < lfu_count; i++) {
            if (recent_counter[lfu_indices[i]] > max_recent) {
                max_recent = recent_counter[lfu_indices[i]];
                lru_index = lfu_indices[i];
            }
        }
        
        return lru_index;
    }
    
    return lfu_indices[0];
}