# Operating Systems Report

## Part 1: Software solution of my assignment:

## C files:
mmsv.c (main file, contains parent thread)
~~~

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
~~~

threads.c (contains threads 1-4 with some validations)
~~~
#include "variables.h"
#include "threads.h"

extern pthread_t last_thread_id;

// Thread 1
// Thread function to validate rows 1-3 and sub-grids 1-3 in the soduku solution file
// it works by validating the rows and sub-grids specificed by 'parameters'

void *validate_thread1(void *args) 
{
    parameters *param = (parameters *)args;
    int start_row = param->start_row; 
    int end_row = param->end_row;   
    int delay = param->delay; 
    thread_ids[0] = pthread_self(); 

    // validate rows from start_row to end_row
    for (int i = start_row; i <= end_row; i++) 
    {
        if (check_validity(Sol[i]))
        {
            pthread_mutex_lock(&lock); 
            validRow[i] = 1; 
            Counter++; 
            pthread_mutex_unlock(&lock); 
          
        }
 
        sleep(delay);  
 
    }
    // validate sub-grids 1-3
    for (int sub = 0; sub < 3; sub++) 
    {
        int subGrid[GRID_SIZE]; 
        int idx = 0;    

        for (int i = sub / 3 * 3; i < sub / 3 * 3 + 3; i++) 
        {
            for (int j = sub % 3 * 3; j < sub % 3 * 3 + 3; j++) 
            {
                subGrid[idx++] = Sol[i][j];
            }
        }

        if (check_validity(subGrid)) 
        {
            pthread_mutex_lock(&lock); 
            validSub[sub] = 1; 
            Counter++;
            pthread_mutex_unlock(&lock);
          
        }
 
        sleep(delay);  
 
    }
    // signal thread parent about completion of this thread's task
    pthread_mutex_lock(&lock);
    completed_threads++;
    // This ensures it's the last thread completing
    if (completed_threads == 4) 
    {  
        last_thread_id = pthread_self();
    }
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&lock);

    
    return NULL;
}

// thread 2
// this function validates rows 4-6 and sub-grids 4-6
// to works by validating rows and sub-grids specificed by 'parmeters'
void *validate_thread2(void *args) 
{
    parameters *param = (parameters *)args;
    int start_row = param->start_row; 
    int end_row = param->end_row; 
    int delay = param->delay; 
    thread_ids[1] = pthread_self(); 

    // Validate rows from start_row to end_row
    for (int i = start_row; i <= end_row; i++) 
    {
        if (check_validity(Sol[i])) 
        {
            pthread_mutex_lock(&lock); 
            validRow[i] = 1; 
            Counter++; 
            pthread_mutex_unlock(&lock); 
         
        }
 
        sleep(delay); 
 
    }

    // Validate sub-grids 4, 5, 6
    for (int sub = 3; sub < 6; sub++) 
    {
        int subGrid[GRID_SIZE]; 
        int idx = 0; 

        for (int i = sub / 3 * 3; i < sub / 3 * 3 + 3; i++) 
        {
            for (int j = sub % 3 * 3; j < sub % 3 * 3 + 3; j++) 
            {
                subGrid[idx++] = Sol[i][j];
            }
        }

        if (check_validity(subGrid)) 
        {
            pthread_mutex_lock(&lock); 
            validSub[sub] = 1; 
            Counter++; 
            pthread_mutex_unlock(&lock); 
            
        }
 
        sleep(delay); 
 
    }
    // signal thread parent about the completion of this Thread's task
    pthread_mutex_lock(&lock);
    completed_threads++;
    // This ensures it's the last thread completing
    if (completed_threads == 4) 
    { 
        last_thread_id = pthread_self();
    }
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&lock);

   
    return NULL;
}


// thread 3
// this function validates rows 7-9 and sub-grids 7-9
// it validates rows and sub-grids specified by 'parameters'
void *validate_thread3(void *args)
{
    parameters *param = (parameters *)args;
    int start_row = param->start_row; 
    int end_row = param->end_row; 
    int delay = param->delay; 
    thread_ids[2] = pthread_self(); 

    // Validate rows from start_row to end_row
    for (int i = start_row; i <= end_row; i++) 
    {
        if (check_validity(Sol[i])) 
        {
            pthread_mutex_lock(&lock); 
            validRow[i] = 1; 
            Counter++; 
            pthread_mutex_unlock(&lock); 
        
        }
 
        sleep(delay);  
 
    }

    // Validate sub-grids 7, 8, and 9
    for (int sub = 6; sub < 9; sub++) 
    {
        int subGrid[GRID_SIZE]; 
        int idx = 0; 

        for (int i = sub / 3 * 3; i < sub / 3 * 3 + 3; i++) 
        {
            for (int j = sub % 3 * 3; j < sub % 3 * 3 + 3; j++) 
            {
                subGrid[idx++] = Sol[i][j];
            }
        }
        // validate sub-grid 7, 8, 9
        if (check_validity(subGrid)) 
        {
            pthread_mutex_lock(&lock); 
            validSub[sub] = 1; 
            Counter++; 
            pthread_mutex_unlock(&lock); 
          
        }

        sleep(delay);  

    }
    // signal thread parent to let parent thread know that this thread completed its task
    pthread_mutex_lock(&lock);
    completed_threads++;
    // This ensures it's the last thread completing
    if (completed_threads == 4) 
    {  
        last_thread_id = pthread_self();
    }
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&lock);

    return NULL;
}
// thread 4
// this thread validates all the columns of the soduku
void *validate_thread4(void *args) 
{
    parameters *param = (parameters *)args;
    int delay = param->delay; 
    thread_ids[3] = pthread_self(); 


    // Validate all cols
    for (int i = 0; i < GRID_SIZE; i++) 
    {
        int column[GRID_SIZE]; 
        for (int j = 0; j < GRID_SIZE; j++) 
        {
            column[j] = Sol[j][i];
        }

        if (check_validity(column)) 
        {
            pthread_mutex_lock(&lock); 
            validCol[i] = 1; 
            Counter++; 
            pthread_mutex_unlock(&lock); 
           
        }
        

        sleep(delay);  

    }
    pthread_mutex_lock(&lock);
    completed_threads++;
    if (completed_threads == 4) 
    { 
        last_thread_id = pthread_self();
    }
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&lock);

    return NULL;
}
~~~
mssv_util.c (includes functions that were originally in mssv and i move it here for code modularity)
~~~
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include "variables.h"
#include "threads.h"
#include "mssv_utils.h"


// this function checks the command line args for validity and sets the delay for thread operations
// it exits the program if the args are not as expected
// parameter argc = number of command line args
// parameter argv = array of command line args string
// parameter delay = pointer to an integer to set the delay
void check_arguments(int argc, char *argv[], int *delay) 
{
    if (argc != 3) 
    {
        fprintf(stderr, "Usage: %s <sudoku_input_file> <delay>\n", argv[0]);
        return 1;
    }

    *delay = atoi(argv[2]);
    if (*delay > 10) 
    {
        fprintf(stderr, "Delay cannot be larger than 10\n");
        return 1;
    }

    printf("Delay set to: %d seconds\n", *delay);
}
// this function initalizes an array of strong messages to hold validation results
// it allocated memory for each message and exits the program if the allocation fails
// the paramter stat_messages = pointer to an array of strings for status messages
void initialize_stat_messages(char **stat_messages) 
{
    for (int i = 0; i < 4; i++) 
    {
        stat_messages[i] = malloc(256);
        if (stat_messages[i] == NULL) 
        {
            fprintf(stderr, "Failed to allocate memory for stat_messages[%d]\n", i);
            return 1;
        }
    }
}
// this function processes and appends validation results to status messages for rows, cols and sub-grids
// the parameter stat_messages = array of strings where validation messages are stored
void process_validation_results(char **stat_messages) 
{
    for (int i = 0; i < 9; i++) 
    {
        if (!validRow[i]) 
        {
            char buffer[100];
            sprintf(buffer, "row %d is invalid, ", i + 1);
            strcat(stat_messages[i / 3], buffer);
        }

        if (!validCol[i]) 
        {
            char buffer[100];
            sprintf(buffer, "column %d is invalid, ", i + 1);
            strcat(stat_messages[3], buffer);
        }

        if (!validSub[i]) 
        {
            char buffer[100];
            sprintf(buffer, "sub-grid %d is invalid, ", i + 1);
            strcat(stat_messages[i / 3], buffer);
        }
    }

}
// frees allocated memory for status messages
// the parameter stat_messages = array of allocated strings to be freed

void cleanup_resources(char **stat_messages) 
{
    for (int i = 0; i < 4; i++) 
    {
        free(stat_messages[i]);
    }
}

// this function prints a final message indicating if the entire soduku is valid based on the validation results
// the parameter total_valid is the total number of valid rows, cols and sub-grids
void final_message(int total_valid) 
{
    const char* validity;
    if (total_valid == 27) 
    {
        validity = "valid";
    } else 
    {
        validity = "invalid";
    }
    printf("There are %d valid rows, columns, and subgrids, therefore the solution is %s.\n", total_valid, validity);
}

// this function finalises the status messages for each thread, removes trailing commas, prints the results
// includes thread indentification in the output
// the parameter stat_messages array of status messages
// the parameter thread_ids array of thread identifiers
void finalize_messages(char *stat_messages[], pthread_t thread_ids[]) 
{
    for (int i = 0; i < 4; i++) 
    {
        if (strstr(stat_messages[i], "invalid") == NULL) 
        {
            strcat(stat_messages[i], "valid");
        } else 
        {
            // Clean up trailing comma
            int len = strlen(stat_messages[i]);
            if (len > 2 && stat_messages[i][len - 2] == ',') 
            {
                stat_messages[i][len - 2] = '\0';
            }
        }
        // print the thread ID while also printing the number of the thread ex: thread 183479147-1, the -1 is saying its thread 1
        printf("Thread %d: Thread ID-%lu-%d: %s\n", i + 1, (unsigned long)thread_ids[i], i + 1, stat_messages[i]);
    }
}
~~~
variables.c (this file includes the variables that i used thruoghout my code)
~~~
#include "variables.h"


// Definitions of structs
int Sol[GRID_SIZE][GRID_SIZE];
int validRow[GRID_SIZE];
int validCol[GRID_SIZE];
int validSub[GRID_SIZE];
int Counter = 0;

// lock mutex
pthread_mutex_t lock;


// Check the validity of a Sudoku grid
int check_validity(int *array) 
{
    int count[GRID_SIZE] = {0};
    for (int i = 0; i < GRID_SIZE; i++) 
    {
        if (array[i] < 1 || array[i] > GRID_SIZE || count[array[i] - 1] > 0)
            return 0;
        count[array[i] - 1]++;
    }
    return 1;
}

    






// Read Sudoku grid from file
void readFromFile(const char *filename) 
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (fscanf(file, "%d", &Sol[i][j]) != 1) 
            {
                fprintf(stderr, "Error reading Sudoku grid from file\n");
                fclose(file);
                exit(EXIT_FAILURE);
            }
        }
    }
    fclose(file);
}
~~~
## header files:
threads.h
~~~
#ifndef THREADS_H
#define THREADS_H

void *validate_thread1(void *args);
void *validate_thread2(void *args);
void *validate_thread3(void *args);
void *validate_thread4(void *args);

#endif
~~~
mssv_utils.h
~~~
#ifndef MSSV_UTILS_H
#define MSSV_UTILS_H

void check_arguments(int argc, char *argv[], int *delay);
void initialize_stat_messages(char **stat_messages);
void process_validation_results(char **stat_messages);
void cleanup_resources(char **stat_messages);
void final_message(int total_valid);
void finalize_messages(char *stat_messages[], pthread_t thread_ids[]);



#endif
~~~
variables.h
~~~
#ifndef VARIABLES_H
#define VARIABLES_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define GRID_SIZE 9

extern int Sol[GRID_SIZE][GRID_SIZE];
extern int validRow[GRID_SIZE];
extern int validCol[GRID_SIZE];
extern int validSub[GRID_SIZE];
extern int Counter;

extern pthread_mutex_t lock;
extern pthread_cond_t cond;
extern int completed_threads;

typedef struct {
    int start_row;
    int end_row;
    int start_col;
    int end_col;
    // int thread_num;
    int delay;
    // int thread_number;
} parameters;



extern pthread_t thread_ids[4];
int check_validity(int *array);
void readFromFile(const char *filename);


#endif
~~~
Makefile(to compile my code)
~~~
CC=gcc
CFLAGS=-Wall -Wextra -g  
LDFLAGS=-pthread         

SRCS=mssv.c threads.c variables.c mssv_utils.c
OBJS=$(SRCS:.c=.o)
DEPS=variables.h threads.h mssv_utils.h

TARGET=mssv

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJS)
~~~

readme.txt
~~~
Author name: Zaid Rushdi Khalaf
Student number: 21467385
Date: 06/05/24

project name: Sudoku Validator
description: this program has been created using multithreads in order to validate a given sudoku file called solution.txt, the main objective was to create 4 threads, each thread was given a certain task to complete. threads 1-3 were responsible of validating 3 rows and 3 sub-grids and thread 4 was responsible of validating all the rows of the sudoku. A parent thread located in the main file (mssv.c) is responsible of making it all work together.

table of contents:
C files:
mssv.c: is my main file and includes my main function that makes everything work

threads.c: includes all the threads that are required to validate the sudoku in addition to some validating messages

variables.c: includes variable declerations and FILE I/O

mssv_utils.c: includes all the functions that simplified the main file and made the code more readable

Header files .h
threads.h

variables.h

mssv_utils.h

Makefile


Head to the Cli:
Usage: clear all object files (make clear)
compile the code: make
enter the following input: ./mssv solution.txt 1 (the number at the end is the delay, you can choose any number you want) it is prefered to choose a higher number which equals a higher delay, the reason behind that is that it will help identify any synchronisation issues

you should be presented with an output like this:
Delay set to: 1 seconds
Thread ID-140455188612672-4 is the last thread.
Thread 1: Thread ID-140455213790784-1: valid
Thread 2: Thread ID-140455205398080-2: valid
Thread 3: Thread ID-140455197005376-3: valid
Thread 4: Thread ID-140455188612672-4: valid
There are 27 valid rows, columns, and subgrids, therefore the solution is valid.

you can also try to check an invalid sudoku, by including the invalid.txt file and you should be presented with an output as follows: ps: feel free to change invalid.txt to your desire in order to check the validator with as many invalid sudoku's as you wish

Delay set to: 4 seconds
Thread ID-140029057812032-4 is the last thread.
Thread 1: Thread ID-140029082990144-1: valid
Thread 2: Thread ID-140029074597440-2: valid
Thread 3: Thread ID-140029066204736-3: row 9 is invalid, sub-grid 9 is invalid
Thread 4: Thread ID-140029057812032-4: column 9 is invalid
There are 24 valid rows, columns, and subgrids, therefore the solution is invalid.

~~~

Detailed Discussion on how Synchronization is acheived when accessing shared resources/ variables and which threads access the shared resources:

Synchronization with with accordance to multithreading refers to the coordination of multiple threads to make sure they access shared resources or critical sections of a code in a controlled manner. If synchronization was not used, then race conditions, data corruption or more unexpected actions could occur. Therfore, i made sure my synchornization was properly implemented.

In my case, Synchronization is critical due to multiple threads accessing and modifying shared resources such as 'validRow' 'validCol' 'validSub' and 'Counter', these shared variables will keep the track of the validity of the rows, columns, sub-grids and the count of validated elements. mutex is used to be able to manage synchronization in my code. pthread_mutex_t lock and a condition variable pthread_cond_t cond.

a mutex is used to make sure that once a thrade is updating shared resources no other thread can access them until the mutex is unclocked. this helps in race conditions where multiple threads are trying to access the same shared resources, leading into a corrupt state.
in my code, each validation function that validates the sudoku, validate_thread1, validate_thread2, validate_thread3, validate_thread4. locks the mutex before updating the shared resources which are: validRow, validCol, validSub, Counter, and then unlocks the mutex immediatley after the update. 
sample below:
~~~
pthread_mutex_lock(&lock);
// Update shared resources
pthread_mutex_unlock(&lock);
~~~
by using this pattern, it ensures that each update to the shared resources is completed fully by one thread before the other thread can start, this process is refered to as (atomic)

throughout my code i also implemented the use of a Condition variable, condition variables are important in multithreaded programs as it enables threads to wait for a particular condition to become true before proceeding.
in my case, the condition variable pthread_cond_t cond is used to synchronize the completion of all threads. the main function located in the mssv.c is used to synchronize the completion of all the 4 threads. the main function will initalize all four threads to validate different parts of the sudoku gird, then it needs to wait until all the 4 threads have completed their tasks before it can proceed to arregate results and print the final validation message, below is a sample of how its handled. firstly each thread sends a signal of its completion by incrementing (completed_threads), then calling the pthread_cond_signal(&cond), inside the locked mutex section. Then, the main function would wait on the condition variable, only when completed_threads is equal to 4 indicating that all 4 threads have been completed.
sample:
~~~
pthread_mutex_lock(&lock);
while (completed_threads < 4) {
    pthread_cond_wait(&cond, &lock); // Wait for all threads to signal completion
}
pthread_mutex_unlock(&lock);
~~~
all of the 4 threads: validate_thread1, validate_thread2, validate_thread3, validate_thread4 access and modify the shared arrays of "validRow", "validCol" "validSub" and Counter. they all use mutex to make sure that their updates do not interfere with each other.
the main thread which is the parent thread located in the main function, waits for all validation threads to complete, aggregates the results, and then prints the final validation message. it uses the condition variable to synchronize on the completion of all validation threads.
