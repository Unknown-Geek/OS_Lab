#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

// Function to perform some CPU-intensive operations
void perform_computation() {
    int i, j;
    double result = 0.0;
    
    for (i = 0; i < 100000000; i++) {
        result += i * 1.0;
    }
    
    // Allocate and free some memory to use system resources
    for (j = 0; j < 10; j++) {
        void *ptr = malloc(1024 * 1024); // Allocate 1MB
        if (ptr) {
            // Use the memory
            for (i = 0; i < 1024 * 1024 / sizeof(int); i++) {
                ((int *)ptr)[i] = i;
            }
            free(ptr);
        }
    }
}

int main() {
    time_t current_time;
    struct tm *time_info;
    char time_string[80];
    struct rusage usage_start, usage_end;
    struct timeval start_time, end_time;
    
    // Get and display current system date and time
    time(&current_time);
    time_info = localtime(&current_time);
    
    printf("Current System Date and Time:\n");
    printf("==============================\n");
    
    strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", time_info);
    printf("Date and Time: %s\n", time_string);
    
    strftime(time_string, sizeof(time_string), "%A, %B %d, %Y", time_info);
    printf("Day: %s\n", time_string);
    
    printf("\n");
    
    // Get resource usage before computation
    getrusage(RUSAGE_SELF, &usage_start);
    gettimeofday(&start_time, NULL);
    
    printf("Now performing some computation...\n");
    perform_computation();
    printf("Computation completed.\n\n");
    
    // Get resource usage after computation
    getrusage(RUSAGE_SELF, &usage_end);
    gettimeofday(&end_time, NULL);
    
    // Calculate elapsed time
    double elapsed_time = (end_time.tv_sec - start_time.tv_sec) + 
                          (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
    
    // Calculate CPU time used in user and system modes
    double user_time = (usage_end.ru_utime.tv_sec - usage_start.ru_utime.tv_sec) + 
                       (usage_end.ru_utime.tv_usec - usage_start.ru_utime.tv_usec) / 1000000.0;
    
    double system_time = (usage_end.ru_stime.tv_sec - usage_start.ru_stime.tv_sec) + 
                         (usage_end.ru_stime.tv_usec - usage_start.ru_stime.tv_usec) / 1000000.0;
    
    // Display resource usage information
    printf("Resource Usage Information:\n");
    printf("===========================\n");
    printf("Real (Elapsed) Time: %.6f seconds\n", elapsed_time);
    printf("User CPU Time: %.6f seconds\n", user_time);
    printf("System CPU Time: %.6f seconds\n", system_time);
    printf("Total CPU Time: %.6f seconds\n", user_time + system_time);
    printf("CPU Utilization: %.2f%%\n", (user_time + system_time) / elapsed_time * 100);
    
    printf("\nMemory Usage:\n");
    printf("Maximum Resident Set Size: %ld KB\n", usage_end.ru_maxrss);
    printf("Page Faults (without I/O): %ld\n", usage_end.ru_minflt - usage_start.ru_minflt);
    printf("Page Faults (with I/O): %ld\n", usage_end.ru_majflt - usage_start.ru_majflt);
    printf("Voluntary Context Switches: %ld\n", usage_end.ru_nvcsw - usage_start.ru_nvcsw);
    printf("Involuntary Context Switches: %ld\n", usage_end.ru_nivcsw - usage_start.ru_nivcsw);
    
    return 0;
}