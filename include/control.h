#ifndef _CONTROL_H_
#define _CONTROL_H_

#include <stdlib.h>
#include <stdio.h>
#include "cmd.h"
#include "common.h"
#include "message.h"
#include "ipc.h"

#define MAX_TURNS 10000

#define TRAIL_VALUE 1
#define DECREASE_FACTOR 0.01

#define POINTS_BIGFOOD 5
#define POINTS_FOOD 1

enum {
	NO_ERROR,
	CTRL_STATE_ZERO,
	CTRL_STATE_STARTING,
	CTRL_STATE_READY,
	
	CTRL_ERR_MEM,
	CTRL_ERR_FOOD,
	CTRL_ERR_ANTHILL,
	CTRL_ERR_TURN
};

enum {
	ANT_INITIALIZED,
	ANT_READY,
	ANT_NEEDHELP,
	ANT_MOVING,
	ANT_DECIDED,
	ANT_STOPPED
};


struct st_ctrl_antinfo{
	int status;
	int id;
	int row;
	int col;
	int carrying;
	int yelled;
	struct st_dir_t dirpointing;
	cmd_t cmd;
};

typedef struct st_ctrl_antinfo * ctrl_antinfo_t;

struct st_ctrl_info {
	ipc_t ipc;
	int status;
	int qtyAnt;
	int rows, cols;
	struct st_dir_t antHill;
	struct st_ctrl_antinfo * ants;
	int qtyFood;
	board_t board;
};

typedef struct st_ctrl_info * ctrl_info_t;


struct ant_and_ctrl_info_st {
	ctrl_info_t ctrl_info;
	int antid;
};

typedef struct ant_and_ctrl_info_st * ant_and_ctrl_info_t;

int launchControl(ipc_t ipc, grid_t gridinfo);
void deleteLaunchControlInfo(ctrl_info_t ctrl_info, handler_f* handlers, cmd_t * cmdLauncher);

int controlLoop(ctrl_info_t ctrl_info, handler_f* handlers, cmd_t * cmdLauncher);
void reqStartAnts(ctrl_info_t ctrl_info, handler_f * handlers);
int playTurn(ctrl_info_t , handler_f*);
int antsStatus(ctrl_info_t ctrl_info, int status);
void decreaseTrail(ctrl_info_t ctrl_info);


int cmpDir(struct st_dir_t * a, struct st_dir_t * b);


void solvePickDecisions(ctrl_info_t ctrl_info);
void solveMoveDecisions(ctrl_info_t ctrl_info);
void sendYellNot(ctrl_info_t ctrl_info);

int calculatePoints(ctrl_info_t ctrl_info);
/* Calculates the points of food collected per Turn, and destroy the food */

ctrl_info_t createCtrlInfo(ipc_t ipc, grid_t gridinfo);
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

int thereIsAnAnt(ctrl_info_t ctrl_info, int row, int col);

cmd_t ctrlHandleStart(void * ctrlInfo , cmd_t cmd);
cmd_t ctrlHandleMove(void * ctrlInfo, cmd_t cmd);
cmd_t ctrlHandleSmell(void * ctrlInfo, cmd_t cmd);
cmd_t ctrlHandlePick(void * ctrlInfo, cmd_t cmd);
cmd_t ctrlHandleAid(void * ctrlInfo, cmd_t cmd);
cmd_t ctrlHandleYell(void * ctrlInfo, cmd_t cmd);
cmd_t ctrlHandleStop(void * ctrlInfo, cmd_t cmd);

#endif