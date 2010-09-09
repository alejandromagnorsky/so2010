#ifndef _parser_
#define _parser_

#include <stdio.h>
#include <stdlib.h>

#define MAX_FILS 80
#define MAX_COLS 80

struct st_grid_t {
    int gridCols;
    int gridRows;
    
    int antCol;
    int antRow;
    
    int cantHor;
    
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
    ERR_HORMIGUERO,
    ERR_CANTCOMCHICA,
    ERR_CANTCOMTOTAL
};


int loadGrid(grid_t tab, char * nombreArchivo);

grid_t gnew();

int alocarComidasChicas(grid_t grid, int cantComChicas);

int alocarComidasGrandes(grid_t grid, int cantComGrandes);

#endif
