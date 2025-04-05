#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

// Structure to represent a page table entry
typedef struct {
    int frame_number;
    int valid;        // 1 = valid, 0 = invalid (page fault)
    int modified;     // 1 = modified, 0 = not modified
    int referenced;   // 1 = referenced, 0 = not referenced
} PageTableEntry;

// Function prototypes
void initialize_page_table(PageTableEntry *page_table, int page_table_size);
int translate_address(uint32_t logical_addr, PageTableEntry *page_table, 
                      int page_size, int *offset, int *page_number);
void print_address_info(uint32_t logical_addr, int physical_addr, 
                        int page_number, int frame_number, int offset);
void print_page_table(PageTableEntry *page_table, int page_table_size);

int main() {
    int page_table_size;    // Number of pages
    int page_size;          // Size of each page in bytes
    int memory_size;        // Size of physical memory in bytes
    uint32_t logical_addr;  // Logical address to translate
    int physical_addr;      // Resulting physical address
    int offset;             // Offset within page
    int page_number;        // Page number from logical address
    int frame_number;       // Frame number from page table
    PageTableEntry *page_table;
    
    // Get page table and memory configuration
    printf("Enter page size (bytes, power of 2): ");
    scanf("%d", &page_size);
    
    printf("Enter physical memory size (bytes): ");
    scanf("%d", &memory_size);
    
    // Calculate number of pages and frames
    page_table_size = memory_size / page_size;
    
    printf("\nPage table size: %d entries\n", page_table_size);
    printf("Page size: %d bytes\n", page_size);
    printf("Physical memory size: %d bytes\n", memory_size);
    
    // Allocate and initialize page table
    page_table = (PageTableEntry*) malloc(page_table_size * sizeof(PageTableEntry));
    initialize_page_table(page_table, page_table_size);
    
    printf("\nInitial Page Table:\n");
    print_page_table(page_table, page_table_size);
    
    // Address translation loop
    while (1) {
        printf("\nEnter logical address to translate (decimal, -1 to quit): ");
        scanf("%u", &logical_addr);
        
        if (logical_addr == (uint32_t)-1) {
            break;
        }
        
        // Translate logical address to physical address
        physical_addr = translate_address(logical_addr, page_table, page_size, &offset, &page_number);
        
        if (physical_addr != -1) {
            frame_number = page_table[page_number].frame_number;
            print_address_info(logical_addr, physical_addr, page_number, frame_number, offset);
        } else {
            printf("Page fault occurred for page %d\n", page_number);
            
            // Simulate page fault handling (assign a random frame)
            page_table[page_number].valid = 1;
            page_table[page_number].frame_number = rand() % page_table_size;
            frame_number = page_table[page_number].frame_number;
            
            printf("Page fault handled: Page %d mapped to frame %d\n", 
                   page_number, frame_number);
            
            // Recalculate physical address after page fault handling
            physical_addr = (frame_number * page_size) + offset;
            print_address_info(logical_addr, physical_addr, page_number, frame_number, offset);
        }
        
        // Update page table for the accessed page
        page_table[page_number].referenced = 1;
        
        // Ask if the page was modified
        int modified;
        printf("Was the page modified? (1=yes, 0=no): ");
        scanf("%d", &modified);
        if (modified) {
            page_table[page_number].modified = 1;
        }
        
        printf("\nUpdated Page Table:\n");
        print_page_table(page_table, page_table_size);
    }
    
    // Free allocated memory
    free(page_table);
    
    return 0;
}

// Initialize page table with random frame numbers and validity bits
void initialize_page_table(PageTableEntry *page_table, int page_table_size) {
    for (int i = 0; i < page_table_size; i++) {
        // Randomly make some pages valid and some invalid
        page_table[i].valid = rand() % 2;
        
        if (page_table[i].valid) {
            // For valid pages, assign a frame number
            page_table[i].frame_number = rand() % page_table_size;
        } else {
            page_table[i].frame_number = -1;  // Invalid frame number
        }
        
        page_table[i].modified = 0;
        page_table[i].referenced = 0;
    }
}

// Translate logical address to physical address
int translate_address(uint32_t logical_addr, PageTableEntry *page_table, 
                     int page_size, int *offset, int *page_number) {
    // Calculate page number and offset
    *page_number = logical_addr / page_size;
    *offset = logical_addr % page_size;
    
    // Check if page is valid
    if (page_table[*page_number].valid) {
        // Calculate physical address
        return (page_table[*page_number].frame_number * page_size) + *offset;
    } else {
        // Page fault
        return -1;
    }
}

// Print address translation information
void print_address_info(uint32_t logical_addr, int physical_addr, 
                       int page_number, int frame_number, int offset) {
    printf("\nAddress Translation:\n");
    printf("Logical Address: %u\n", logical_addr);
    printf("  Page Number: %d\n", page_number);
    printf("  Offset: %d\n", offset);
    printf("Physical Address: %d\n", physical_addr);
    printf("  Frame Number: %d\n", frame_number);
    printf("  Offset: %d\n", offset);
}

// Print the current state of the page table
void print_page_table(PageTableEntry *page_table, int page_table_size) {
    printf("Page# | Frame# | Valid | Modified | Referenced\n");
    printf("-------------------------------------------\n");
    
    for (int i = 0; i < page_table_size; i++) {
        printf("%-5d | ", i);
        
        if (page_table[i].valid) {
            printf("%-6d | ", page_table[i].frame_number);
        } else {
            printf("%-6s | ", "N/A");
        }
        
        printf("%-5d | ", page_table[i].valid);
        printf("%-8d | ", page_table[i].modified);
        printf("%-10d\n", page_table[i].referenced);
    }
}