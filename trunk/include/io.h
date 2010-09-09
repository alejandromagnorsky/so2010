#ifndef _io_
#define _io_

#include <stdio.h>
#include <stdlib.h>

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


int loadGrid(grid_t tab, char * fileName);

grid_t gnew();

int allocSmallFood(grid_t grid, int smallFoodQuant);

int allocBigFood(grid_t grid, int bigFoodQuant);

#endif
