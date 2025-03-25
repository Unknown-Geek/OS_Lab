#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/resource.h>

void print_usage()
{
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    printf("User time : %ld.%06ld seconds\n", usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
    printf("Kernel time : %ld.%06ld seconds\n", usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
}

int main()
{
    printf("Current date and time : \n");
    system("date");

    printf("Initial resource usage : \n");
    print_usage();

    long i;
    long max = 1000000000;

    // CPU intensive loop
    for (i = 0; i < max; i++)
    {
        // Do nothing
    }

    printf("Final resource usage : \n");
    print_usage();

    return 0;
}