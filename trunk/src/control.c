#include "../include/control.h"


int main(){
	
}

int launchControl(grid_t gridinfo){
	
	ctrl_info_t ctrl_info = createCtrlInfo(gridinfo);
	handler_f* handlers = buildControlHandlerArray();
	cmd_t * cmdLauncher = createCmdLauncher(ctrl_info->qtyAnt);
	if(ctrl_info->status != NO_ERROR){
		deleteControlInfo(ctrl_info, handlers, cmdLauncher);
		return ctrl_info->status;
	}
	if(handlers == NULL){
		deleteControlInfo(ctrl_info, handlers, cmdLauncher);
		return CTRL_ERR_MEM;
	}
	if(cmdLauncher == NULL){
		deleteControlInfo(ctrl_info, handlers, cmdLauncher);
		return CTRL_ERR_MEM;
	}
	
	controlLoop(ctrl_info, handlers, cmdLauncher);
}

void deleteLaucnhControlInfo(ctrl_info_t ctrl_info, handler_f* handlers, cmd_t * cmdLauncher){
	deleteCtrlInfo(ctrl_info);
	deleteControlHandlerArray(handlers);
	deleteCmdLauncher(cmdLauncher);
}



int controlLoop(ctrl_info_t ctrl_info, handler_f* handlers, cmd_t * cmdLauncher){
	int turn = 0;
	int points = 0;
	
	ctrl_info->status = CTRL_STATE_ZERO;
	reqStartAnts(ctrl_info, handlers);
	
	while(turn < MAX_TURNS && ctrl_info->qtyFood > 0){
		
	}
	
	return points;
	
}

int reqStartAnts(ctrl_info_t ctrl_info, handler_f * handlers){
	int i;
	for(i = 0; i < ctrl_info->qtyAnt; i++){
		
	}
}



ctrl_info_t createCtrlInfo(grid_t gridinfo){
	ctrl_info_t ret = (ctrl_info_t) malloc(sizeof(struct st_ctrl_info));
	if(ret == NULL){
		return NULL;
	}
	
	ret->rows = gridinfo->gridRows;
	ret->cols = gridinfo->gridCols;
	ret->qtyAnt = gridinfo->antsQuant;
	ret->qtyFood = gridinfo->smallFoodQuant + gridinfo->bigFoodQuant;
	
	ret->board = createBoard(ret->rows, ret->cols);
	if(ret->board == NULL){
		ret->status = CTRL_ERR_MEM;
		return ret;
	}
	
	ret->ants = createAntInfo(ret->qtyAnt, gridinfo);
	if(ret->ants == NULL){
		deleteBoard(ret->board, ret->rows, ret->cols);
		ret->status = CTRL_ERR_MEM;
		return ret;
	}
	
	if(fillWithFood(ret, gridinfo->smallFoodQuant, gridinfo->bigFoodQuant, 
			gridinfo->smallFoods, gridinfo->bigFoods) != NO_ERROR){
		deleteBoard(ret->board, ret->rows, ret->cols);
		deleteAntInfo(ret->ants);
		ret->status = CTRL_ERR_FOOD;
		return ret;
	}
	
	if(locateAntHill(ret, gridinfo->anthillRow, gridinfo->anthillCol) != NO_ERROR){
		deleteBoard(ret->board, ret->rows, ret->cols);
		deleteAntInfo(ret->ants);
		ret->status = CTRL_ERR_ANTHILL;
		return ret;
	}
	
	return ret;
}

void deleteCtrlInfo(ctrl_info_t ctrl_info){
	deleteBoard(ctrl_info->board, ctrl_info->rows, ctrl_info->cols);
	deleteAntInfo(ctrl_info->ants);
	free(ctrl_info);
}



board_t createBoard(int rows, int cols){
	
	board_t board = (board_t) malloc(sizeof(struct tile_t*) * rows);
	if(board == NULL){
		return NULL;
	}

	int i, j;
	for(i = 0; i < rows; i++){
		board[i] = (struct tile_t * ) malloc(sizeof(struct tile_t) * cols);
		if(board[i] == NULL){
			for(j = 0; j < i; j++){
				free(board[i]);
			}
			free(board);
			return NULL;
		}
		for(j = 0; j < cols; j++){
			board[i][j].obj = NO_OBJ;
			board[i][j].trail = 0;
		}
	}
	return board;
}

void deleteBoard(board_t board, int rows, int cols){
	int i;
	for(i = 0; i < rows; i++){
		free(board[i]);
	}
	free(board);
}



cmd_t * createCmdLauncher(int qtyAnt){
	return calloc(qtyAnt, sizeof(cmd_t));
	
}

void deleteCmdLauncher(cmd_t * cmdLauncher){
	free(cmdLauncher);
}



struct st_ctrl_antinfo * createAntInfo(int qtyAnt, grid_t gridinfo){
	struct st_ctrl_antinfo * ants = calloc(qtyAnt, sizeof(struct st_ctrl_antinfo));
	if(ants != NULL){
		int i = 0;
		for(i = 0; i < qtyAnt; i++){
			ants[i].row = gridinfo->anthillRow;
			ants[i].col = gridinfo->anthillCol;
			ants[i].status = ANT_INITIALIZED;
		}
	}
	return ants;
}

void deleteAntInfo(struct st_ctrl_antinfo * antinfo){
	free(antinfo);
}


int locateAntHill(ctrl_info_t ret, int row, int col){
	if(row >= ret->rows || col >= ret->cols){
		return CTRL_ERR_ANTHILL;
	}
	ret->antHill.row = row;
	ret->antHill.col = col;
	ret->board[row][col].obj = OBJ_ANTHILL;
	return NO_ERROR;
}


int fillWithFood(ctrl_info_t ctrl_info, int qtySmallFood, int qtyBigFood, int * smallFoods, int * bigFoods){
	int i;
	for(i = 0; i < qtySmallFood; i++){
		if(smallFoods[i] >= ctrl_info->rows || smallFoods[i+1] >= ctrl_info->cols ){
			return CTRL_ERR_FOOD;
		}
		ctrl_info->board[smallFoods[i]][smallFoods[i + 1]].obj = OBJ_FOOD;
	}
	for(i = 0; i < qtyBigFood; i++){
		if(bigFoods[i] >= ctrl_info->rows || bigFoods[i+1] >= ctrl_info->cols ){
			return CTRL_ERR_FOOD;
		}
		ctrl_info->board[bigFoods[i]][bigFoods[i + 1]].obj = OBJ_BIGFOOD;
	}
	return NO_ERROR;
}


handler_f* buildControlHandlerArray(ctrl_info_t ctrl_info){
	handler_f* ctrlHandlers = buildAntHandlerArray();
	ctrlHandlers[CMD_START] = ctrlHandleStart;
	ctrlHandlers[CMD_MOVE_REQ] = ctrlHandleMove;
	ctrlHandlers[CMD_SMELL_REQ] = ctrlHandleSmell;
	ctrlHandlers[CMD_PICK_REQ] = ctrlHandlePick;
	ctrlHandlers[CMD_AID_REQ] = ctrlHandleAid;
	ctrlHandlers[CMD_YELL_REQ] = ctrlHandleYell;
	ctrlHandlers[CMD_STOP] = ctrlHandleStop;
}

void deleteControlHandlerArray(handler_f* ctrlhandlers){
	free(ctrlhandlers);
}



cmd_t ctrlHandleStart(void * ctrlInfo , cmd_t cmd){
	return newStart();
}

cmd_t ctrlHandleMove(void * ctrlInfo, cmd_t cmd){
	return NULL;
}

cmd_t ctrlHandleSmell(void * ctrlInfo, cmd_t cmd){
	return NULL;
}

cmd_t ctrlHandlePick(void * ctrlInfo, cmd_t cmd){
	return NULL;
}

cmd_t ctrlHandleAid(void * ctrlInfo, cmd_t cmd){
	return NULL;
}

cmd_t ctrlHandleYell(void * ctrlInfo, cmd_t cmd){
	return NULL;
}

cmd_t ctrlHandleStop(void * ctrlInfo, cmd_t cmd){
	return NULL;
}