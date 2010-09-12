#include "../include/control.h"

int main(){

}

int launchControl(ipc_t ipc, grid_t gridinfo){
	
	ctrl_info_t ctrl_info = createCtrlInfo(ipc, gridinfo);
	handler_f* handlers = buildControlHandlerArray();
	cmd_t * cmdLauncher = createCmdLauncher(ctrl_info->qtyAnt);
	if(ctrl_info->status != NO_ERROR){
		deleteCtrlInfo(ctrl_info);
		return ctrl_info->status;
	}
	if(handlers == NULL){
		deleteCtrlInfo(ctrl_info);
		return CTRL_ERR_MEM;
	}
	if(cmdLauncher == NULL){
		deleteCtrlInfo(ctrl_info);
		return CTRL_ERR_MEM;
	}
	
	controlLoop(ctrl_info, handlers, cmdLauncher);
}

void deleteLaunchControlInfo(ctrl_info_t ctrl_info, handler_f* handlers, cmd_t * cmdLauncher){
	deleteCtrlInfo(ctrl_info);
	deleteControlHandlerArray(handlers);
	deleteCmdLauncher(cmdLauncher);
}



int controlLoop(ctrl_info_t ctrl_info, handler_f* handlers, cmd_t * cmdLauncher){
	int turn = 0;
	int points = 0;
	int ans;
	ctrl_info->status = CTRL_STATE_ZERO;
	
	reqStartAnts(ctrl_info, handlers);
	
	while(turn < MAX_TURNS && ctrl_info->qtyFood > 0){
		if((ans = playTurn(ctrl_info, handlers) < 0)){
			return CTRL_ERR_TURN;
		}else{
			points += ans;
		}
		turn++;
	}
	
	return points;
}

void reqStartAnts(ctrl_info_t ctrl_info, handler_f * handlers){
	int i, antsPrepared = 0;
	message_t msg;
	
	struct ant_and_ctrl_info_st info;
	
	ctrl_info->status = CTRL_STATE_STARTING;
	
	
	while(antsStatus(ctrl_info, ANT_READY) != ctrl_info->qtyAnt){
		if((msg = recvMessage(ctrl_info->ipc)) != NULL){
			ctrl_info->ants[mfrom(msg) - FIRST_ANT_ID].id = mfrom(msg);
			dispatchCmd(&info, (cmd_t) mdata(msg), handlers);
		}
	}
	cmd_t startcmd = newStart();
	int size = sizeof(struct cmd_start_t);
	for(i = FIRST_ANT_ID; i < ctrl_info->qtyAnt + FIRST_ANT_ID; i++){
		message_t msg = mnew(0, i, sizeof(struct cmd_start_t), (char *) &startcmd);
		sendMessage(ctrl_info->ipc, msg);
		mdel(msg);
	}
	ctrl_info->status = CTRL_STATE_READY;
}

int antsStatus(ctrl_info_t ctrl_info, int status){
	int i, ret = 0;
	for(i = 0; i < ctrl_info->qtyAnt; i++){
		ret += (ctrl_info[i].status) == status ? 1 : 0;
	}
	return ret;
}

int playTurn(ctrl_info_t ctrl_info, handler_f * handlers){
	int i;
	message_t msg;
	struct ant_and_ctrl_info_st info;
	
	decreaseTrail(ctrl_info);
	
	//SEND TURN REQUESTS
	cmd_t cmdTurn = newTurn();
	for(i = 0; i < ctrl_info->qtyAnt; i++){
		msg = mnew(0, ctrl_info->ants[i].id, sizeof(struct cmd_turn_t), (char *) &cmdTurn);
		sendMessage(ctrl_info->ipc, msg);
		mdel(msg);
	}
	
	//RECV RESPONSES
	while(antsStatus(ctrl_info, ANT_READY) != 0){
		if((msg = recvMessage(ctrl_info->ipc)) != NULL){
			info.ctrl_info = ctrl_info;
			info.antid = msg->header.from;
			dispatchCmd(&info, (cmd_t) msg->data, handlers);		
		}
	}
	
	solveConcurrencyDecisions(ctrl_info);
	
	if(antsStatus(ctrl_info, ANT_STOPPED) == ctrl_info->qtyAnt){
		return 0;
	}
	
	//SEND RESPONSES
	for(i = 0; i < ctrl_info->qtyAnt; i++){
		switch(ctrl_info->ants[i].cmd->type){
		case CMD_MOVE_RES:
			msg = mnew(0, ctrl_info->ants[i].id, sizeof(struct cmd_move_res_t), (char *) &ctrl_info->ants[i].cmd);
			break;
		case CMD_SMELL_RES:
			msg = mnew(0, ctrl_info->ants[i].id, sizeof(struct cmd_smell_res_t), (char *) &ctrl_info->ants[i].cmd);
			break;
		case CMD_PICK_RES:
			msg = mnew(0, ctrl_info->ants[i].id, sizeof(struct cmd_pick_res_t), (char *) &ctrl_info->ants[i].cmd);
			break;
		case CMD_AID_RES:
			msg = mnew(0, ctrl_info->ants[i].id, sizeof(struct cmd_aid_res_t), (char *) &ctrl_info->ants[i].cmd);
			break;
		case CMD_YELL_RES:
			msg = mnew(0, ctrl_info->ants[i].id, sizeof(struct cmd_yell_res_t), (char *) &ctrl_info->ants[i].cmd);
			break;
		case CMD_YELL_NOT:
			msg = mnew(0, ctrl_info->ants[i].id, sizeof(struct cmd_yell_not_t), (char *) &ctrl_info->ants[i].cmd);
			break;
		}
		sendMessage(ctrl_info->ipc, msg);
		mdel(msg);
	}
	
	for(i = 0; i < ctrl_info->qtyAnt; i++){
		ctrl_info->ants[i].status = ANT_READY;
	}
	
	//CALCULATE POINTS
}

void decreaseTrail(ctrl_info_t ctrl_info){
	int i,j;
	for(i = 0; i < ctrl_info->rows; i++){
		for(j = 0; j < ctrl_info->cols; j++){
			ctrl_info->board[i][j].trail -= (ctrl_info->board[i][j].trail) == 0 ? 0 : DECREASE_FACTOR;
		}
	}
}

void solveConcurrencyDecisions(ctrl_info_t ctrl_info){
	int i, j;
	for(i = 0; i < ctrl_info->qtyAnt; i++){
		if(ctrl_info->ants[i].status == ANT_NEEDHELP){
			for(j = 0; j < ctrl_info->qtyAnt; j++){
				if( i != j && (ctrl_info->ants[j].status == ANT_NEEDHELP || ctrl_info->ants[j].status == ANT_GIVINGHELP)){
					if(ctrl_info->ants[j].dirpointing.row == ctrl_info->ants[i].row && 
							ctrl_info->ants[j].dirpointing.col == ctrl_info->ants[i].col){
						ctrl_info->ants[i].status == ANT_DECIDED;
						ctrl_info->ants[j].status = ANT_DECIDED;
						ctrl_info->ants[i].cmd = newPickRes(STATUS_OK);
						if(ctrl_info->ants[j].status == ANT_GIVINGHELP){
							ctrl_info->ants[j].cmd = newAidRes(STATUS_OK);
						}else{
							ctrl_info->ants[j].cmd = newPickRes(STATUS_SOLVED);
						}
					}
				}
			}
		}
	}
	for(i = 0; i < ctrl_info->qtyAnt; i++){
		if(ctrl_info->ants[i].status == ANT_NEEDHELP || ctrl_info->ants[i].status == ANT_GIVINGHELP){
			ctrl_info->ants[i].cmd = ctrl_info->ants[i].status == ANT_NEEDHELP ? newPickRes(STATUS_FAILED) : newAidRes(STATUS_FAILED);
			ctrl_info->ants[i].status = ANT_DECIDED;
		}
	}
}



ctrl_info_t createCtrlInfo(ipc_t ipc, grid_t gridinfo){
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
	ret->ipc = ipc;
	
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
			ants[i].id = i + FIRST_ANT_ID;
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
	handler_f * ctrlHandlers = buildHandlerArray();
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


int thereIsAnAnt(ctrl_info_t ctrl_info, int row, int col){
	int i;
	for(i = 0; i < ctrl_info->qtyAnt; i++){
		if(ctrl_info->ants[i].row == row && ctrl_info->ants[i].col == col){
			return 1;
		}
	}
	return 0;
}


cmd_t ctrlHandleStart(void * ptrInfo , cmd_t cmd){
	cmd_start_t cmdreq = (cmd_start_t) cmd;
	ant_and_ctrl_info_t info = (ant_and_ctrl_info_t) ptrInfo;
	
	info->ctrl_info->ants[info->antid - FIRST_ANT_ID].id = info->antid;
	info->ctrl_info->ants[info->antid - FIRST_ANT_ID].status = ANT_READY;
	info->ctrl_info->ants[info->antid - FIRST_ANT_ID].cmd = newStart();
	return NULL;
}


cmd_t ctrlHandleMove(void * ptrInfo, cmd_t cmd){
	cmd_move_req_t cmdreq = (cmd_move_req_t) cmd;
	ant_and_ctrl_info_t info = (ant_and_ctrl_info_t) ptrInfo;
	
	int mov[8] = {-1,0, 0,1, 1,0, 0,-1};
	
	struct st_dir_t nextPos;
	nextPos.row = info->ctrl_info->ants[info->antid - FIRST_ANT_ID].row + mov[cmdreq->dir];
	nextPos.col = info->ctrl_info->ants[info->antid - FIRST_ANT_ID].col + mov[cmdreq->dir+1];
	
	if(nextPos.row >= info->ctrl_info->rows && nextPos.col >= info->ctrl_info->cols){
		info->ctrl_info->ants[info->antid - FIRST_ANT_ID].status = ANT_DECIDED;
		info->ctrl_info->ants[info->antid - FIRST_ANT_ID].cmd = newMoveRes(STATUS_FAILED);
		return NULL;
	}
	
	int i;
	for(i = 0; i < info->ctrl_info->qtyAnt; i++){
		if(info->ctrl_info->ants[i].cmd->type == CMD_MOVE_RES &&
				info->ctrl_info->ants[i].row == nextPos.row && 
					info->ctrl_info->ants[i].col == nextPos.col){
			info->ctrl_info->ants[i].status = ANT_DECIDED;
			info->ctrl_info->ants[i].cmd = newMoveRes(STATUS_FAILED);
			return NULL;
		}
	}
	
	struct st_dir_t currPos;
	currPos.row = info->ctrl_info->ants[info->antid - FIRST_ANT_ID].row;
	currPos.col = info->ctrl_info->ants[info->antid - FIRST_ANT_ID].col;
	
	info->ctrl_info->board[currPos.row][currPos.col].trail = TRAIL_VALUE;
	
	info->ctrl_info->ants[i].row = nextPos.row;
	info->ctrl_info->ants[i].col = nextPos.col;
	info->ctrl_info->ants[i].cmd = newMoveRes(cmdreq->dir);
	info->ctrl_info->ants[i].status = ANT_DECIDED;
	return NULL;
}


cmd_t ctrlHandleSmell(void * ptrInfo, cmd_t cmd){
	cmd_smell_req_t cmdreq = (cmd_smell_req_t) cmd;
	ant_and_ctrl_info_t info = (ant_and_ctrl_info_t) ptrInfo;
	
	
	struct st_dir_t currPos;
	currPos.row = info->ctrl_info->ants[info->antid].row;
	currPos.col = info->ctrl_info->ants[info->antid].col;
	
	tile_t tileRes = calloc(8, sizeof(struct tile_t));
	
	int mov[16] = {-1,0, -1,1 , 0,1, 1,1, 1,0, 1,-1, 0,-1, -1,-1};
	
	int i;
	for(i = 0; i < 16; i = i + 2){
		if(currPos.row + mov[i] < info->ctrl_info->rows && currPos.col + mov[i+1]){
			if(thereIsAnAnt(info->ctrl_info, currPos.row + mov[i], currPos.col + mov[i+1]) ){
				tileRes[i/2].obj = OBJ_ANT;
			}else{
				tileRes[i/2].obj = info->ctrl_info->board[currPos.row + mov[i]][currPos.col + mov[i+1]].obj;
				tileRes[i/2].trail = info->ctrl_info->board[currPos.row + mov[i]][currPos.col + mov[i+1]].trail;
			}
		}else{
			tileRes[i/2].obj = OBJ_OUT_OF_BOUNDS;
		}
	}
	
	info->ctrl_info->ants[info->antid - FIRST_ANT_ID].cmd = newSmellRes(tileRes);
	info->ctrl_info->ants[info->antid - FIRST_ANT_ID].status = ANT_DECIDED;
	return NULL;
}


cmd_t ctrlHandlePick(void * ptrInfo, cmd_t cmd){
	cmd_pick_req_t cmdreq = (cmd_pick_req_t) cmd;
	ant_and_ctrl_info_t info = (ant_and_ctrl_info_t) ptrInfo;
	
	struct st_dir_t currPos;
	currPos.row = info->ctrl_info->ants[info->antid - FIRST_ANT_ID].row;
	currPos.col = info->ctrl_info->ants[info->antid - FIRST_ANT_ID].col;
	
	int mov[8] = {-1,0, 0,1, 1,0, 0,-1};
	
	struct st_dir_t foodPos;
	foodPos.row = info->ctrl_info->ants[info->antid - FIRST_ANT_ID].row + mov[cmdreq->dir];
	foodPos.col = info->ctrl_info->ants[info->antid - FIRST_ANT_ID].col + mov[cmdreq->dir+1];
	
	if(foodPos.row >= info->ctrl_info->rows || foodPos.col >= info->ctrl_info->cols){
		info->ctrl_info->ants[info->antid - FIRST_ANT_ID].status = ANT_DECIDED;
		info->ctrl_info->ants[info->antid - FIRST_ANT_ID].cmd = newPickRes(STATUS_FAILED);
		return NULL;
	}
	
	if(info->ctrl_info->board[foodPos.row][foodPos.col].obj == OBJ_FOOD){
		info->ctrl_info->board[foodPos.row][foodPos.col].obj = NO_OBJ;
		info->ctrl_info->ants[info->antid - FIRST_ANT_ID].cmd = newPickRes(STATUS_OK);
		info->ctrl_info->ants[info->antid - FIRST_ANT_ID].status = ANT_DECIDED;
		return NULL;
	}
	if(info->ctrl_info->board[foodPos.row][foodPos.col].obj == OBJ_BIGFOOD){
		info->ctrl_info->ants[info->antid - FIRST_ANT_ID].dirpointing.row = foodPos.row;
		info->ctrl_info->ants[info->antid - FIRST_ANT_ID].dirpointing.col = foodPos.col;
		info->ctrl_info->ants[info->antid - FIRST_ANT_ID].status = ANT_NEEDHELP;
		return NULL;
	}
	info->ctrl_info->ants[info->antid - FIRST_ANT_ID].status = ANT_DECIDED;
	info->ctrl_info->ants[info->antid - FIRST_ANT_ID].cmd = newPickRes(STATUS_FAILED);
	return NULL;
}

cmd_t ctrlHandleAid(void * ptrInfo, cmd_t cmd){
	cmd_aid_req_t cmdreq = (cmd_aid_req_t) cmd;
	ant_and_ctrl_info_t info = (ant_and_ctrl_info_t) ptrInfo;
	
	int mov[8] = {-1,0, 0,1, 1,0, 0,-1};
	
	struct st_dir_t foodPos;
	foodPos.row = info->ctrl_info->ants[info->antid - FIRST_ANT_ID].row + mov[cmdreq->dir];
	foodPos.col = info->ctrl_info->ants[info->antid - FIRST_ANT_ID].col + mov[cmdreq->dir+1];
	
	if(foodPos.row >= info->ctrl_info->rows || foodPos.col >= info->ctrl_info->cols){
		info->ctrl_info->ants[info->antid - FIRST_ANT_ID].status = ANT_DECIDED;
		info->ctrl_info->ants[info->antid - FIRST_ANT_ID].cmd = newAidRes(STATUS_FAILED);
		return NULL;
	}
	
	info->ctrl_info->ants[info->antid - FIRST_ANT_ID].dirpointing.row = foodPos.row;
	info->ctrl_info->ants[info->antid - FIRST_ANT_ID].dirpointing.col = foodPos.col;
	info->ctrl_info->ants[info->antid - FIRST_ANT_ID].status = ANT_GIVINGHELP;
	return NULL;
}

cmd_t ctrlHandleYell(void * ptrInfo, cmd_t cmd){
	cmd_yell_req_t cmdreq = (cmd_yell_req_t) cmd;
	ant_and_ctrl_info_t info = (ant_and_ctrl_info_t) ptrInfo;
	
	cmd_t cmdnot = newYellNot();
	message_t msg;
	int i;
	for(i = 0; i < info->ctrl_info->qtyAnt; i++){
		if(info->ctrl_info->ants[info->antid - FIRST_ANT_ID].id != info->antid){
			msg = mnew(0, info->antid, sizeof(struct cmd_yell_not_t), (char *) &cmdnot);
			sendMessage(info->ctrl_info->ipc, msg);
			mdel(msg);
		}
	}
	
	cmd_t cmdres = newYellRes();
	info->ctrl_info->ants[info->antid - FIRST_ANT_ID].status = ANT_DECIDED;
	return NULL;
}

cmd_t ctrlHandleStop(void * ptrInfo, cmd_t cmd){
	ant_and_ctrl_info_t info = (ant_and_ctrl_info_t) ptrInfo;
	info->ctrl_info->ants[info->antid - FIRST_ANT_ID].status = ANT_STOPPED;
	return NULL;
}
