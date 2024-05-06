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



