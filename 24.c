#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

#define SIZE 1024

int main(int argc, char *argv[])
{
    // Check if we have the correct number of arguments
    if (argc != 4)
    {
        printf("Usage: %s <virtual space in KB> <page size in KB> <virtual address>\n", argv[0]);
        return 1;
    }

    int vspace = atoi(argv[1]);
    int psize = atoi(argv[2]);
    unsigned int addr = atoi(argv[3]);

    int pages = (vspace * 1024) / psize;
    psize = psize * 1024;

    int page = addr / psize;
    int offset = addr % psize;

    if (page >= pages)
    {
        printf("Invalid Page Number!\n");
        return 1;
    }

    // Initialize random seed
    srand(time(NULL));

    // Only need pages entries in the page table, not SIZE
    int page_table[pages];

    // Only initialize entries that are actually needed
    for (int i = 0; i < pages; i++)
    {
        if (rand() % 10 == 0)
        {
            page_table[i] = -1; // Page fault
        }
        else
        {
            // Frames should be allocated separately from page numbers
            // Let's create a fixed number of frames (e.g., equal to pages for simplicity)
            page_table[i] = rand() % pages;
        }
    }

    int frame = page_table[page];

    if (frame == -1)
        printf("Page Miss!\n");
    else
    {
        printf("Page: %d, Offset: %d\n", page, offset);
        printf("Physical: <%d, %d>\n", frame, offset);
    }

    return 0;
}