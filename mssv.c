
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include "variables.h"
#include "threads.h"
#include "mssv_utils.h"

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int completed_threads = 0;
pthread_t thread_ids[4]; // store thread IDs for identification
pthread_t last_thread_id; // store ID of the last thread

int main(int argc, char *argv[]) 
{
    pthread_t tid1, tid2, tid3, tid4; // thread identifiers
    int delay;
    int total_valid = 0; 
    char *stat_messages[4];

    // check and extract args
    check_arguments(argc, argv, &delay);
    initialize_stat_messages(stat_messages);
    pthread_mutex_init(&lock, NULL);
    
    parameters param1 = {0, 2, delay, 1}; // parameters for thread 1
    parameters param2 = {3, 5, delay, 2}; // parameters for thread 2
    parameters param3 = {6, 8, delay, 3}; // parameters for thread 3
    parameters param4 = {0, 8, delay, 4}; // parameters for thread 4

    // read sudoku grid from file
    readFromFile(argv[1]);  

    // Capture thread ID right after creation
    pthread_create(&tid1, NULL, validate_thread1, &param1);
    thread_ids[0] = tid1;  

    pthread_create(&tid2, NULL, validate_thread2, &param2);
    thread_ids[1] = tid2;  

    pthread_create(&tid3, NULL, validate_thread3, &param3);
    thread_ids[2] = tid3;  

    pthread_create(&tid4, NULL, validate_thread4, &param4);
    thread_ids[3] = tid4;  

    // the threads will send a signal to the parent thread to let it know that the thread has finished its job
    pthread_mutex_lock(&lock);
    while (completed_threads < 4) 
    { 
    pthread_cond_wait(&cond, &lock); // Wait for all threads to signal completion
    }
    pthread_mutex_unlock(&lock);

    // here we are waiting for the threads to complete their tasks
    pthread_join(tid1, NULL); 
    pthread_join(tid2, NULL); 
    pthread_join(tid3, NULL);
    pthread_join(tid4, NULL);

    printf("Thread ID-%lu-%d is the last thread.\n", (unsigned long)last_thread_id, 4); 

    sleep(delay); 

    process_validation_results(stat_messages);
    finalize_messages(stat_messages, thread_ids);
    total_valid = Counter;
    final_message(total_valid); 
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);


    return 0;
}
