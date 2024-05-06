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


