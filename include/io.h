#ifndef _io_
#define _io_

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h> 

#define MAX_FILS 60
#define MAX_COLS 60

struct st_grid_t {
    int gridCols;
    int gridRows;
    
    int anthillCol;
    int anthillRow;
    
    int antsQuant;
    
    int smallFoodQuant;
    int bigFoodQuant;
    
    int * smallFoods;
    int * bigFoods;
};

typedef struct st_grid_t* grid_t;


enum {
	NO_ERRORS,
    ERR_FILE,
    MEMORY_ERROR,
    ERR_ANTHILL,
    ERR_FOODPOSITION
};


int loadGrid(grid_t grid, char * fileName);
/* Loads the grid information from the specified file */

grid_t gnew();
/* Allocs a new grid, but doesnt reserve space for the big and small food arrays */

int allocSmallFood(grid_t grid, int smallFoodQuant);

int allocBigFood(grid_t grid, int bigFoodQuant);

int checkFoodPositions(grid_t grid);

int initializeScreen(grid_t grid);

void addcharat(int col, int row, char c);


#endif
