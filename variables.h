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
