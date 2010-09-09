#ifndef _parser_
#define _parser_

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
    ERR_SMALLFOODQUANT,
    ERR_BIGFOODQUANT,
    ERR_CANTCOMTOTAL
};


int loadGrid(grid_t tab, char * nombreArchivo);

grid_t gnew();

int allocSmallFood(grid_t grid, int cantComChicas);

int allocBigFood(grid_t grid, int cantComGrandes);

#endif
