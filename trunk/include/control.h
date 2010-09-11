#ifndef SERVER_H_
#define SERVER_H_

#include <stdlib.h>
#include <stdio.h>
#include "io.h"
#include "logic.h"
#include "common.h"

#define MAX_TURNS 10000

void initControl(board_t board, grid_t gridinfo);
board_t createBoard(int rows, int cols);
struct st_dir_info* createAntPosArray(int qtyAnt, grid_t gridinfo);
grid_t getGridInfo(char * filename);
void fillWithFood(board_t board, grid_t gridinfo);

#endif
