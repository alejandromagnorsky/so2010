#include "../include/control.h"


int main(){
	
}

int launchControl(grid_t gridinfo){
	
	ctrl_info_t ctrl_info = createCtrlInfo(gridinfo);
	handler_f* handlers = buildControlHandlerArray();
	if(ctrl_info != NO_ERROR){
		return ctrl_info;
	}
	
}

void controlLoop(){
	
}

ctrl_info_t createCtrlInfo(grid_t gridinfo){
	ctrl_info_t ret = malloc(sizeof(struct st_ctrl_info));
	if(ret == NULL){
		return NULL;
	}
	
	ret->rows = gridinfo->gridRows;
	ret->cols = gridinfo->gridCols;
	ret->qtyAnt = gridinfo->antsQuant;
	
	ret->board= createBoard(ret->rows, ret->cols);
	if(ret->board == NULL){
		ret->status = CTRL_ERR_MEM;
		return ret;
	}
	
	ret->ants = createAntPosArray(qtyAnt, gridinfo);
	if(ret->ants == NULL){
		deleteBoard(ret->board, ret->rows, ret->cols);
		ret->status = CTRL_ERR_MEM;
		return ret;
	}
	
	if(fillWithFood(ret, gridinfo->smallFoodQuant, gridinfo->bigFoodQuant, 
			gridinfo->smallFoods, gridinfo->bigFoods) != NO_ERROR){
		deleteBoard(ret->board, ret->rows, ret->cols);
		ret->status = CTRL_ERROR_FOOD;
		return ret;
	}
	return ret;
}

void deleteCtrlInfo(ctrl_info_t ctrl_info){
	deleteBoard(ctrl_info->board);
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

struct st_dir_info* createAntPosArray(int qtyAnt, grid_t gridinfo){
	struct st_dir_info* ants = calloc(qtyAnt, sizeof(struct st_dir_info));
	if(ants != NULL){
		int i = 0;
		for(i = 0; i < qtyAnt; i++){
			ants[i].row = gridinfo->anthillRow;
			ants[i].col = gridinfo->anthillCol;
		}
	}
	return ants;
}


int fillWithFood(ctrl_info_t ctrl_info, int qtySmallFood, int qtyBigFood, int * smallFoods, int * bigFoods){
	int i;
	for(i = 0; i < qtySmallFood; i++){
		if(smallFoods[i] >= ctrl_info->rows || smallFoods[i+1] >= ctrl_info->cols ){
			return ERROR_FOOD;
		}
		board[smallFoods[i]][smallFoods[i + 1]].obj = OBJ_FOOD;
	}
	for(i = 0; i < qtyBigFood; i++){
		if(bigFoods[i] >= ctrl_info->rows || bigFoods[i+1] >= ctrl_info->cols ){
			return ERROR_FOOD;
		}
		board[gridinfo->bigFoods[i]][gridinfo->bigFoods[i + 1]].obj = OBJ_BIGFOOD;
	}
	return NO_ERROR;
}


handler_f* buildControlHandlerArray(ctrl_info_t ctrl_info){
	handler_f* ctrlHandlers = buildHandlerArray();
	ctrlHandlers[CMD_START] = ctrlHandleStart;
	ctrlHandlers[CMD_MOVE_REQ] = ctrlHandleMove;
	ctrlHandlers[CMD_SMELL_REQ] = ctrlHandleSmell;
	ctrlHandlers[CMD_PICK_REQ] = ctrlHandlePick;
	ctrlHandlers[CMD_AID_REQ] = ctrlHandleAid;
	ctrlHandlers[CMD_YELL_REQ] = ctrlHandleYell;
	ctrlHandlers[CMD_STOP] = ctrlHandleStop;
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