#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define MAX_ELEMENTS 1000

// Shared data for threads
double numbers[MAX_ELEMENTS];
int count;
double mean, median, std_dev;

// Thread functions
void *calculate_mean(void *arg);
void *calculate_median(void *arg);
void *calculate_std_dev(void *arg);

// Utility function for sorting (used for median calculation)
int compare_doubles(const void *a, const void *b);

int main() {
    pthread_t mean_thread, median_thread, std_dev_thread;
    int i;
    
    // Read number of elements
    printf("Enter the number of elements (max %d): ", MAX_ELEMENTS);
    scanf("%d", &count);
    
    if (count <= 0 || count > MAX_ELEMENTS) {
        printf("Invalid number of elements\n");
        return 1;
    }
    
    // Read the elements
    printf("Enter %d numbers:\n", count);
    for (i = 0; i < count; i++) {
        scanf("%lf", &numbers[i]);
    }
    
    // Create threads for different calculations
    pthread_create(&mean_thread, NULL, calculate_mean, NULL);
    pthread_create(&median_thread, NULL, calculate_median, NULL);
    pthread_create(&std_dev_thread, NULL, calculate_std_dev, NULL);
    
    // Wait for threads to complete
    pthread_join(mean_thread, NULL);
    pthread_join(median_thread, NULL);
    pthread_join(std_dev_thread, NULL);
    
    // Display results
    printf("\nStatistical Measures:\n");
    printf("---------------------\n");
    printf("Mean: %.4f\n", mean);
    printf("Median: %.4f\n", median);
    printf("Standard Deviation: %.4f\n", std_dev);
    
    return 0;
}

// Thread function to calculate mean
void *calculate_mean(void *arg) {
    int i;
    double sum = 0;
    
    printf("Mean calculation started in thread %lu\n", pthread_self());
    
    for (i = 0; i < count; i++) {
        sum += numbers[i];
    }
    
    mean = sum / count;
    
    printf("Mean calculation completed: %.4f\n", mean);
    pthread_exit(NULL);
}

// Thread function to calculate median
void *calculate_median(void *arg) {
    double sorted_numbers[MAX_ELEMENTS];
    
    printf("Median calculation started in thread %lu\n", pthread_self());
    
    // Copy the numbers to avoid modifying the original array
    for (int i = 0; i < count; i++) {
        sorted_numbers[i] = numbers[i];
    }
    
    // Sort the array
    qsort(sorted_numbers, count, sizeof(double), compare_doubles);
    
    // Calculate median
    if (count % 2 == 0) {
        // Even number of elements
        median = (sorted_numbers[count/2 - 1] + sorted_numbers[count/2]) / 2.0;
    } else {
        // Odd number of elements
        median = sorted_numbers[count/2];
    }
    
    printf("Median calculation completed: %.4f\n", median);
    pthread_exit(NULL);
}

// Thread function to calculate standard deviation
void *calculate_std_dev(void *arg) {
    double sum_squared_diff = 0.0;
    
    printf("Standard deviation calculation started in thread %lu\n", pthread_self());
    
    // Wait for mean to be calculated (as we need it for std dev)
    pthread_join(pthread_self(), NULL);
    
    for (int i = 0; i < count; i++) {
        sum_squared_diff += pow(numbers[i] - mean, 2);
    }
    
    std_dev = sqrt(sum_squared_diff / count);
    
    printf("Standard deviation calculation completed: %.4f\n", std_dev);
    pthread_exit(NULL);
}

// Comparison function for qsort
int compare_doubles(const void *a, const void *b) {
    double arg1 = *(const double*)a;
    double arg2 = *(const double*)b;
    
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}