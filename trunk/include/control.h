#ifndef CONTROL_H_
#define CONTROL_H_

#include <stdlib.h>
#include <stdio.h>
#include "io.h"
#include "logic.h"

#define MAX_TURNS 10000

enum {
	NO_ERROR,
	ERROR_MEM,
	ERROR_FOOD
};

struct st_ctrl_info {
	int qtyAnt;
	int rows, cols;
	struct st_dir_info* ants;
	board_t board;
};

typedef struct st_ctrl_info * ctrl_info_t;

struct st_dir_info{
	int row;
	int col;
};

typedef struct st_dir_info * dir_info_t;

typedef struct tile_t ** board_t;

ctrl_info_t createCtrlInfo(grid_t gridinfo);
board_t createBoard(int rows, int cols);
void freeBoard(board_t board, int rows, int cols);
struct st_dir_info* createAntPosArray(int qtyAnt, grid_t gridinfo);
int fillWithFood(ctrl_info_t ctrl_info, int qtySmallFood, int qtyBigFood, int * smallFoods, int * bigFoods);

handler_f* buildControlHandlerArray();

cmd_t ctrlHandleStart(void * ctrlInfo , cmd_t cmd);
cmd_t ctrlHandleMove(void * ctrlInfo, cmd_t cmd);


#endif