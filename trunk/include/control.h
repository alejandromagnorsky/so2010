#ifndef CONTROL_H_
#define CONTROL_H_

#include <stdlib.h>
#include <stdio.h>
#include "io.h"
#include "ipc.h"

#define MAX_TURNS 10000

enum {
	NO_ERROR,
	CTRL_STATE_ZERO,
	CTRL_STATE_STARTING,
	CTRL_STATE_READY,
	
	CTRL_ERR_MEM,
	CTRL_ERR_FOOD,
	CTRL_ERR_ANTHILL
};

enum {
	ANT_INITIALIZED,
	ANT_READY,
};

typedef struct tile_t ** board_t;

struct st_dir_t{
	int row;
	int col;
};

struct st_ctrl_antinfo{
	int status;
	int row;
	int col;
};
typedef struct st_ctrl_antinfo * ctrl_antinfo_t;

struct st_ctrl_info {
	int status;
	int qtyAnt;
	int rows, cols;
	struct st_dir_t antHill;
	struct st_ctrl_antinfo * ants;
	int qtyFood;
	board_t board;
};

typedef struct st_ctrl_info * ctrl_info_t;


int launchControl(grid_t gridinfo);
void deleteLaucnhControlInfo(ctrl_info_t ctrl_info, handler_f* handlers, cmd_t * cmdLauncher);

int controlLoop(ctrl_info_t ctrl_info, handler_f* handlers, cmd_t * cmdLauncher);

ctrl_info_t createCtrlInfo(grid_t gridinfo);
void deleteCtrlInfo(ctrl_info_t ctrl_info);

board_t createBoard(int rows, int cols);
void deleteBoard(board_t board, int rows, int cols);

struct st_ctrl_antinfo * createAntInfo(int qtyAnt, grid_t gridinfo);
void deleteAntInfo(struct st_ctrl_antinfo * antinfo);

cmd_t * createCmdLauncher(int qtyAnt);
void deleteCmdLauncher(cmd_t * cmdLauncher);

handler_f* buildControlHandlerArray();
void deleteControlHandlerArray(handler_f* ctrlhandlers);

int fillWithFood(ctrl_info_t ctrl_info, int qtySmallFood, int qtyBigFood, int * smallFoods, int * bigFoods);

cmd_t ctrlHandleStart(void * ctrlInfo , cmd_t cmd);
cmd_t ctrlHandleMove(void * ctrlInfo, cmd_t cmd);
cmd_t ctrlHandleSmell(void * ctrlInfo, cmd_t cmd);
cmd_t ctrlHandlePick(void * ctrlInfo, cmd_t cmd);
cmd_t ctrlHandleAid(void * ctrlInfo, cmd_t cmd);
cmd_t ctrlHandleYell(void * ctrlInfo, cmd_t cmd);
cmd_t ctrlHandleStop(void * ctrlInfo, cmd_t cmd);

#endif