#ifndef SERVER_H_
#define SERVER_H_

#include <stdlib.h>
#include <stdio.h>
#include "io.h"
#include "logic.h"

#define MAX_TURNS 10000

struct st_dir_info{
	int row;
	int col;
};

typedef struct st_dir_info * dir_info_t;

typedef struct tile_t ** board_t;


board_t createBoard(int rows, int cols);
struct st_dir_info* createAntPosArray(int qtyAnt, grid_t gridinfo);
grid_t getGridInfo(char * filename);
void fillWithFood(board_t board, grid_t gridinfo);

#endif