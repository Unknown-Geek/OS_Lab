#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

#define SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <virtual space in MB> <page size in KB> <virtual address>\n", argv[0]);
        return 1;
    }

    int vspace = atoi(argv[1]);
    int psize = atoi(argv[2]);
    unsigned int addr = atoi(argv[3]);

    int pages = (vspace * 1024) / psize;
    psize = psize * 1024;

    int page = addr / psize;
    int offset = addr % psize;

    if (page >= pages) {
        printf("Invalid Page Number!\n");
        return 1;
    }

    srand(time(NULL));

    int page_table[pages];

    for (int i = 0; i < pages; i++) {
        if (rand() % 10 == 0) {
            page_table[i] = -1;
        }
        else {
            page_table[i] = rand() % pages;
        }
    }

    int frame = page_table[page];

    if (frame == -1)
        printf("Page Miss!\n");
    else {
        printf("Page: %d, Offset: %d\n", page, offset);
        printf("Physical: <%d, %d>\n", frame, offset);
    }

    return 0;
}
