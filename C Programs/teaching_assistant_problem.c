#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_STUDENTS 5
#define MAX_WAITING 3     // Maximum number of chairs in the hallway

// Semaphores
sem_t ta_sleep;          // Semaphore to wake up the TA
sem_t student_ready;     // Student is ready to be helped
sem_t ta_helping;        // TA is helping a student
sem_t mutex;             // Mutex for modifying waiting students count

// Global variables
int waiting_students = 0; // Number of waiting students
int next_student_id = 1;  // ID for next student thread

// Function prototypes
void *ta_function(void *param);
void *student_function(void *param);

int main() {
    pthread_t ta_thread;
    pthread_t student_threads[NUM_STUDENTS];
    int student_ids[NUM_STUDENTS];
    
    // Initialize semaphores
    sem_init(&mutex, 0, 1);           // Mutex starts at 1
    sem_init(&ta_sleep, 0, 0);        // TA starts sleeping (0)
    sem_init(&student_ready, 0, 0);   // No students ready initially
    sem_init(&ta_helping, 0, 0);      // TA not helping initially
    
    printf("Teaching Assistant Problem Simulation\n");
    printf("=====================================\n");
    
    // Create TA thread
    pthread_create(&ta_thread, NULL, ta_function, NULL);
    
    // Create student threads
    for (int i = 0; i < NUM_STUDENTS; i++) {
        student_ids[i] = next_student_id++;
        pthread_create(&student_threads[i], NULL, student_function, &student_ids[i]);
        
        // Sleep to stagger student arrivals
        sleep(rand() % 3 + 1);
    }
    
    // Wait for student threads to finish
    for (int i = 0; i < NUM_STUDENTS; i++) {
        pthread_join(student_threads[i], NULL);
    }
    
    // Terminate the program (TA thread will be terminated)
    printf("\nAll students have been helped. Terminating the program.\n");
    exit(0);
    
    // Clean up semaphores
    sem_destroy(&mutex);
    sem_destroy(&ta_sleep);
    sem_destroy(&student_ready);
    sem_destroy(&ta_helping);
    
    return 0;
}

// TA thread function
void *ta_function(void *param) {
    while (1) {
        // TA sleeps until a student arrives
        printf("TA: Going to sleep\n");
        sem_wait(&ta_sleep);  // Sleep until a student wakes TA
        
        // Critical section
        sem_wait(&mutex);
        waiting_students--;
        sem_post(&mutex);
        
        // TA is awake and ready to help student
        printf("TA: Helping a student (waiting students: %d)\n", waiting_students);
        sem_post(&student_ready);  // Signal that TA is ready to help
        
        // Simulate helping a student
        sleep(rand() % 5 + 1);
        
        // Finished helping student
        sem_post(&ta_helping);     // Signal that TA is done helping
    }
    
    return NULL;
}

// Student thread function
void *student_function(void *param) {
    int student_id = *((int*)param);
    
    printf("Student %d: Trying to get help from the TA\n", student_id);
    
    // Critical section
    sem_wait(&mutex);
    
    if (waiting_students < MAX_WAITING) {
        waiting_students++;
        printf("Student %d: Waiting in the hallway (waiting students: %d)\n", 
               student_id, waiting_students);
        
        // Wake up the TA
        sem_post(&mutex);
        sem_post(&ta_sleep);
        
        // Wait for TA to be ready
        sem_wait(&student_ready);
        
        printf("Student %d: Getting help from the TA\n", student_id);
        
        // Wait for TA to finish helping
        sem_wait(&ta_helping);
        
        printf("Student %d: Finished getting help from the TA\n", student_id);
    } else {
        // No chairs available, student leaves
        printf("Student %d: No chairs available, will try again later\n", student_id);
        sem_post(&mutex);
        
        // Try again after some time
        sleep(rand() % 5 + 5);
        student_function(param);  // Recursive call to try again
    }
    
    return NULL;
}