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
