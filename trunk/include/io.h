#ifndef _io_
#define _io_

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "../include/common.h"
#include "../include/cmd.h"

#define MAX_FILS 60
#define MAX_COLS 60


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
/* Allocs a new grid, but doesnt reserve space for the big and 
	small food arrays */

int allocSmallFood(grid_t grid, int smallFoodQuant);
/* Allocs the space needed for the small food positions */

int allocBigFood(grid_t grid, int bigFoodQuant);
/* Allocs the space needed for the big food positions */

int checkFoodPositions(grid_t grid);
/* Checks if the food given positions are inside the map and 
	in available places */

int initializeScreen(grid_t grid);
/* Initializes the screen with the information from the input file */


void addCharAt(int col, int row, char c);
/* Adds the given character in the specified positions, but taking 
	into account the margins */
	
void addDoubleAt(int col, int row, double num);
/* Adds the given number in the specified positions, but taking 
	into account the margins */
	
void refreshGrid(board_t board, grid_t grid);
/* Refreshes the contents of the board */

#endif
