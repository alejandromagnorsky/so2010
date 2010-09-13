#include "../include/control.h"
#include <time.h>

int launchControl(ipc_t ipc, grid_t gridinfo){
	
	ctrl_info_t ctrl_info = createCtrlInfo(ipc, gridinfo);
	handler_f* handlers = buildControlHandlerArray();
	cmd_t * cmdLauncher = createCmdLauncher(ctrl_info->qtyAnt);
	if(ctrl_info->status != NO_ERROR){
		LOGPID("ERRORS WITH CTRL INFO\n");
		return ctrl_info->status;
	}
	if(handlers == NULL){
		LOGPID("ERRORS WITH CTRL HANDLERS\n");
		return CTRL_ERR_MEM;
	}
	if(cmdLauncher == NULL){
		LOGPID("ERRORS WITH CTRL MEM\n");
		return CTRL_ERR_MEM;
	}
	
	//initializeScreen(gridinfo);
	
	controlLoop(ctrl_info, handlers, cmdLauncher, gridinfo);
	addStringAt(gridinfo->gridRows + 5 + 4,0,"Finish with points: ");
	printw("%d\n", ctrl_info->points);
	getch();
	endwin();
	return NO_ERROR;
}

void deleteLaunchControlInfo(ctrl_info_t ctrl_info, handler_f* handlers, cmd_t * cmdLauncher){
	deleteCtrlInfo(ctrl_info);
	deleteControlHandlerArray(handlers);
	deleteCmdLauncher(cmdLauncher);
}



int controlLoop(ctrl_info_t ctrl_info, handler_f* handlers, cmd_t * cmdLauncher, grid_t gridinfo){
	LOGPID("Start control Loop\n");
	int ans;
	ctrl_info->status = CTRL_STATE_ZERO;
	
	reqStartAnts(ctrl_info, handlers);
	
	while(ctrl_info->turn < MAX_TURNS && ctrl_info->points < ctrl_info->qtyFoodPoints){
		//fprintf(stderr, "[%d] Called playTurn.\n", clock());

		if((ans = playTurn(ctrl_info, handlers) < 0)) {
			return CTRL_ERR_TURN;
		}else{
			ctrl_info->points += ans;
		}
		//fprintf(stderr, "[%d] Returned from playTurn.\n", clock());
		ctrl_info->turn++;
		
		//fprintf(stderr, "[%d] Called refreshGrid.\n", clock());	
		//refreshGrid(ctrl_info, gridinfo);
		//fprintf(stderr, "[%d] Returned from refreshGrid.\n", clock());
	}

	return NO_ERROR;
}

void reqStartAnts(ctrl_info_t ctrl_info, handler_f * handlers){
	
	
	int i, aux, antsPrepared = 0;
	message_t msg;
	
	struct ant_and_ctrl_info_st info;
	
	ctrl_info->status = CTRL_STATE_STARTING;

	//LOGPID("[%d] Waiting to receive CMD_START_T\n", clock());
	
	while((aux = antsStatus(ctrl_info, ANT_READY)) != ctrl_info->qtyAnt){
		if((msg = recvMessage(ctrl_info->ipc)) != NULL){
			if(mfrom(msg) >= ctrl_info->qtyAnt + FIRST_ANT_ID || mfrom(msg) < FIRST_ANT_ID){
				LOGPID("Receive SPAM. ID: %d\n", mfrom(msg));
			}else{
				LOGPID("Receive CMD: %d from: %d\n", ((cmd_t) mdata(msg))->type, mfrom(msg));
				info.ctrl_info = ctrl_info;
				info.antid = mfrom(msg);
				ctrl_info->ants[mfrom(msg) - FIRST_ANT_ID].id = mfrom(msg);
				dispatchCmd(&info, (cmd_t) mdata(msg), handlers);
			}
		}
	}
	
	//LOGPID("[%d] Received all CMD_START_T\n", clock());
	
	cmd_t startcmd = newStart();
	int size = sizeof(struct cmd_start_t);
	for(i = 0; i < ctrl_info->qtyAnt; i++){
		LOGPID("Send CMD_START_T to ant: %d\n", ctrl_info->ants[i].id);
		msg = mnew(SERVER_ID, ctrl_info->ants[i].id, sizeof(struct cmd_start_t), (char *) startcmd);
		sendMessage(ctrl_info->ipc, msg);
		mdel(msg);
	}
	
	ctrl_info->status = CTRL_STATE_READY;
}

int antsStatus(ctrl_info_t ctrl_info, int status){
	int i, ret = 0;
	for(i = 0; i < ctrl_info->qtyAnt; i++){
		ret += (ctrl_info->ants[i].status) == status ? 1 : 0;
	}
	return ret;
}

int playTurn(ctrl_info_t ctrl_info, handler_f * handlers){
    clock_t start;
	//LOGPID("[%d] Turn start\n", start = clock());
	int i;
	message_t msg;
	struct ant_and_ctrl_info_st info;
	
	decreaseTrail(ctrl_info);
	
	//SEND TURN REQUESTS
	cmd_t cmdTurn = newTurn();
	for(i = 0; i < ctrl_info->qtyAnt; i++){
		msg = mnew(SERVER_ID, ctrl_info->ants[i].id, sizeof(struct cmd_turn_t), (char *) cmdTurn);
		LOGPID("Send CMD_TURN to antid: %d\n", ctrl_info->ants[i].id);
		sendMessage(ctrl_info->ipc, msg);
		mdel(msg);
	}
	
	//RECV RESPONSES
	while(antsStatus(ctrl_info, ANT_READY) != 0){
		if((msg = recvMessage(ctrl_info->ipc)) != NULL){
			info.ctrl_info = ctrl_info;
			info.antid = msg->header.from;
			LOGPID("Control: Receive CMD_TYPE: %d, from: %d\n", ((cmd_t) msg->data)->type, info.antid);
			dispatchCmd(&info, (cmd_t) msg->data, handlers);	
		}
	}
	
	LOGPID("\n\n\n   DECICIERON TODOS      \n\n\n");
	
	solvePickDecisions(ctrl_info);
	solveMoveDecisions(ctrl_info);
	
	//SEND RESPONSES
	for(i = 0; i < ctrl_info->qtyAnt; i++){
		switch(ctrl_info->ants[i].cmd->type){
		case CMD_MOVE_RES:
			msg = mnew(SERVER_ID, ctrl_info->ants[i].id, sizeof(struct cmd_move_res_t), (char *) ctrl_info->ants[i].cmd);
			break;
		case CMD_SMELL_RES:
			msg = mnew(SERVER_ID, ctrl_info->ants[i].id, sizeof(struct cmd_smell_res_t), (char *) ctrl_info->ants[i].cmd);
			break;
		case CMD_PICK_RES:
			msg = mnew(SERVER_ID, ctrl_info->ants[i].id, sizeof(struct cmd_pick_res_t), (char *) ctrl_info->ants[i].cmd);
			break;
		case CMD_AID_RES:
			msg = mnew(SERVER_ID, ctrl_info->ants[i].id, sizeof(struct cmd_aid_res_t), (char *) ctrl_info->ants[i].cmd);
			break;
		case CMD_YELL_RES:
			msg = mnew(SERVER_ID, ctrl_info->ants[i].id, sizeof(struct cmd_yell_res_t), (char *) ctrl_info->ants[i].cmd);
			break;
		case CMD_YELL_NOT:
			msg = mnew(SERVER_ID, ctrl_info->ants[i].id, sizeof(struct cmd_yell_not_t), (char *) ctrl_info->ants[i].cmd);
			break;
		}
		sendMessage(ctrl_info->ipc, msg);
		mdel(msg);
	}
	
	sendYellNot(ctrl_info);
	
	for(i = 0; i < ctrl_info->qtyAnt; i++){
		ctrl_info->ants[i].status = ANT_READY;
	}
	LOGPID("[-] Turn end. Took %.2fs.\n", (clock() - start) / (double) CLOCKS_PER_SEC);
	return calculatePoints(ctrl_info);
}


void sendYellNot(ctrl_info_t ctrl_info){
	int i, j;
	message_t msg;
	cmd_t cmdnot;
	for(i=0; i < ctrl_info->qtyAnt; i++){
		if(ctrl_info->ants[i].yelled){
			for(j = 0; j < ctrl_info->qtyAnt; j++){
				cmdnot = newYellNot(ctrl_info->ants[i].row, ctrl_info->ants[i].col);
				msg = mnew(SERVER_ID, ctrl_info->ants[j].id, sizeof(struct cmd_yell_not_t), (char *) cmdnot);
				sendMessage(ctrl_info->ipc, msg);
			}
		}
	}
}


int calculatePoints(ctrl_info_t ctrl_info){
	int i;
	int ret = 0;
	struct st_dir_t antDir;
	for(i = 0; i < ctrl_info->qtyAnt; i++){
		antDir.row = ctrl_info->ants[i].row;
		antDir.col = ctrl_info->ants[i].col;
		if(cmpDir(&antDir, &ctrl_info->antHill)){
			if(ctrl_info->ants[i].carrying == OBJ_BIGFOOD){
				ret += POINTS_BIGFOOD;
			}else if(ctrl_info->ants[i].carrying == OBJ_FOOD){
				ret += POINTS_FOOD;
			}
			ctrl_info->ants[i].carrying = NO_OBJ;
		}
	}
}

void decreaseTrail(ctrl_info_t ctrl_info){
	int i,j;
	for(i = 0; i < ctrl_info->rows; i++){
		for(j = 0; j < ctrl_info->cols; j++){
			ctrl_info->board[i][j].trail -= (ctrl_info->board[i][j].trail) == 0 ? 0 : DECREASE_FACTOR;
		}
	}
}

void solvePickDecisions(ctrl_info_t ctrl_info){
	LOGPID("Control: Solving Move Decisions\n");
	int i, j;
	for(i = 0; i < ctrl_info->qtyAnt; i++){
		if(ctrl_info->ants[i].status == ANT_NEEDHELP){
			for(j = 0; j < ctrl_info->qtyAnt; j++){
				if( i != j && (ctrl_info->ants[j].status == ANT_NEEDHELP)){
					if(ctrl_info->ants[j].dirpointing.row == ctrl_info->ants[i].row && 
							ctrl_info->ants[j].dirpointing.col == ctrl_info->ants[i].col){
						ctrl_info->ants[i].status = ANT_DECIDED;
						ctrl_info->ants[j].status = ANT_DECIDED;
						ctrl_info->ants[i].carrying = OBJ_BIGFOOD;
						ctrl_info->ants[i].cmd = newPickRes(STATUS_OK);
						ctrl_info->ants[j].cmd = newPickRes(STATUS_SOLVED);
						}
					}
				}
			}
		}
	for(i = 0; i < ctrl_info->qtyAnt; i++){
		if(ctrl_info->ants[i].status == ANT_NEEDHELP){
			ctrl_info->ants[i].cmd = newPickRes(STATUS_SOLVED);
			ctrl_info->ants[i].status = ANT_DECIDED;
		}
	}
}

void solveMoveDecisions(ctrl_info_t ctrl_info){
	int i, j;
	for(i = 0; i < ctrl_info->qtyAnt; i++){
		if(ctrl_info->ants[i].status == ANT_MOVING){
			if(cmpDir(&ctrl_info->ants[i].dirpointing, &ctrl_info->antHill)){
				ctrl_info->ants[i].status = ANT_DECIDED;
				ctrl_info->ants[i].row = ctrl_info->ants[i].dirpointing.row;
				ctrl_info->ants[i].col = ctrl_info->ants[i].dirpointing.col;
			}else{
				for(j = i; j < ctrl_info->qtyAnt; j++){
					if(ctrl_info->ants[j].status == ANT_MOVING){
						if(cmpDir(&ctrl_info->ants[i].dirpointing, &ctrl_info->ants[j].dirpointing)){
							if(ctrl_info->ants[i].carrying == OBJ_BIGFOOD || 
									(ctrl_info->ants[i].carrying == OBJ_FOOD && ctrl_info->ants[i].carrying != OBJ_BIGFOOD) || 
									(ctrl_info->ants[i].carrying == NO_OBJ && ctrl_info->ants[i].carrying == NO_OBJ)){
								ctrl_info->ants[i].cmd = newMoveRes(STATUS_OK);
								ctrl_info->ants[j].cmd = newMoveRes(STATUS_FAILED);
								ctrl_info->board[ctrl_info->ants[i].row][ctrl_info->ants[i].col].trail = TRAIL_VALUE;
								ctrl_info->ants[i].row = ctrl_info->ants[i].dirpointing.row;
								ctrl_info->ants[i].col = ctrl_info->ants[i].dirpointing.col;
								LOGPID("Control: MOVE SUCCESSFUL\n");
								ctrl_info->ants[i].status = ANT_DECIDED;
								ctrl_info->ants[j].status = ANT_DECIDED;
							}else{
								ctrl_info->ants[j].cmd = newMoveRes(STATUS_OK);
								ctrl_info->ants[i].cmd = newMoveRes(STATUS_FAILED);
								ctrl_info->board[ctrl_info->ants[j].row][ctrl_info->ants[j].col].trail = TRAIL_VALUE;
								ctrl_info->ants[j].row = ctrl_info->ants[j].dirpointing.row;
								ctrl_info->ants[j].col = ctrl_info->ants[j].dirpointing.col;
								LOGPID("Control: MOVE SUCCESSFUL\n");
								ctrl_info->ants[i].status = ANT_DECIDED;
								ctrl_info->ants[j].status = ANT_DECIDED;
							}
						}else if(ctrl_info->ants[i].dirpointing.row == ctrl_info->ants[j].row && ctrl_info->ants[i].dirpointing.col == ctrl_info->ants[j].col && 
								ctrl_info->ants[j].dirpointing.row == ctrl_info->ants[i].row && ctrl_info->ants[j].dirpointing.col == ctrl_info->ants[i].col){
							ctrl_info->ants[i].cmd = newMoveRes(STATUS_OK);
							ctrl_info->ants[j].cmd = newMoveRes(STATUS_OK);
							ctrl_info->board[ctrl_info->ants[i].row][ctrl_info->ants[i].col].trail = TRAIL_VALUE;
							ctrl_info->board[ctrl_info->ants[j].row][ctrl_info->ants[j].col].trail = TRAIL_VALUE;
							ctrl_info->ants[j].row = ctrl_info->ants[j].dirpointing.row;
							ctrl_info->ants[j].col = ctrl_info->ants[j].dirpointing.col;
							ctrl_info->ants[i].row = ctrl_info->ants[i].dirpointing.row;
							ctrl_info->ants[i].col = ctrl_info->ants[i].dirpointing.col;
							LOGPID("Control: MOVE SUCCESSFUL\n");
							ctrl_info->ants[i].status = ANT_DECIDED;
							ctrl_info->ants[j].status = ANT_DECIDED;
						}
					}
				}
				//If nobody is pointing to the same place:
				ctrl_info->ants[i].status = ANT_DECIDED;
				ctrl_info->ants[i].row = ctrl_info->ants[i].dirpointing.row;
				ctrl_info->ants[i].col = ctrl_info->ants[i].dirpointing.col;
				LOGPID("Control: MOVE SUCCESSFUL\n");
			}
		}
	}
}

int cmpDir(struct st_dir_t * a, struct st_dir_t * b){
	return a->row == b->row && a->col == b->col;
}

ctrl_info_t createCtrlInfo(ipc_t ipc, grid_t gridinfo){
	ctrl_info_t ret = (ctrl_info_t) malloc(sizeof(struct st_ctrl_info));
	if(ret == NULL){
		return NULL;
	}
	
	ret->rows = gridinfo->gridRows;
	ret->cols = gridinfo->gridCols;
	ret->qtyAnt = gridinfo->antsQuant;
	LOG("Cantidad de hormigas: %d\n", ret->qtyAnt);
	ret->turn = 0;
	ret->points = 0;
	ret->qtyFoodPoints = (gridinfo->smallFoodQuant * POINTS_FOOD) + (gridinfo->bigFoodQuant * POINTS_BIGFOOD);
	
	ret->board = createBoard(ret->rows, ret->cols);
	if(ret->board == NULL){
		ret->status = CTRL_ERR_MEM;
		return ret;
	}
	
	ret->ants = createAntInfo(ret->qtyAnt, gridinfo);
	if(ret->ants == NULL){
		//deleteBoard(ret->board, ret->rows, ret->cols);
		ret->status = CTRL_ERR_MEM;
		return ret;
	}
	
	if(fillWithFood(ret, gridinfo->smallFoodQuant, gridinfo->bigFoodQuant, 
			gridinfo->smallFoods, gridinfo->bigFoods) != NO_ERROR){
		//deleteBoard(ret->board, ret->rows, ret->cols);
		//deleteAntInfo(ret->ants);
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
	for(i = 0; i < qtySmallFood * 2; i = i + 2){
		if(smallFoods[i+1] >= ctrl_info->rows || smallFoods[i] >= ctrl_info->cols ){
			return CTRL_ERR_FOOD;
		}
		ctrl_info->board[smallFoods[i+1]][smallFoods[i]].obj = OBJ_FOOD;
	}
	for(i = 0; i < qtyBigFood * 2; i = i + 2){
		if(bigFoods[i+1] >= ctrl_info->cols || bigFoods[i] >= ctrl_info->cols ){
			return CTRL_ERR_FOOD;
		}
		ctrl_info->board[bigFoods[i+1]][bigFoods[i]].obj = OBJ_BIGFOOD;
	}
	return NO_ERROR;
}



handler_f* buildControlHandlerArray(ctrl_info_t ctrl_info){
	handler_f * ctrlHandlers = buildHandlerArray();
	ctrlHandlers[CMD_START] = ctrlHandleStart;
	ctrlHandlers[CMD_MOVE_REQ] = ctrlHandleMove;
	ctrlHandlers[CMD_SMELL_REQ] = ctrlHandleSmell;
	ctrlHandlers[CMD_PICK_REQ] = ctrlHandlePick;
	ctrlHandlers[CMD_YELL_REQ] = ctrlHandleYell;
	ctrlHandlers[CMD_STOP] = ctrlHandleStop;
	
	return ctrlHandlers;
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
	
	LOGPID("Control: Handle CMD_START, type: %d\n", cmdreq->type);
	
	info->ctrl_info->ants[info->antid - FIRST_ANT_ID].id = info->antid;
	info->ctrl_info->ants[info->antid - FIRST_ANT_ID].status = ANT_READY;
	info->ctrl_info->ants[info->antid - FIRST_ANT_ID].cmd = newStart();
	return NULL;
}



cmd_t ctrlHandleMove(void * ptrInfo, cmd_t cmd){
	cmd_move_req_t cmdreq = (cmd_move_req_t) cmd;
	ant_and_ctrl_info_t info = (ant_and_ctrl_info_t) ptrInfo;
	
	LOGPID("Control: Handle CMD_MOVE, type: %d, dir: %d\n", cmdreq->type, cmdreq->dir);
	
	int mov[8] = {-1,0, 0,1, 1,0, 0,-1};
	
	struct st_dir_t nextPos;
	nextPos.row = info->ctrl_info->ants[info->antid - FIRST_ANT_ID].row + mov[cmdreq->dir];
	nextPos.col = info->ctrl_info->ants[info->antid - FIRST_ANT_ID].col + mov[cmdreq->dir+1];
	
	//LOGPID("\n\nControl: Current Position: %d, %d\n", info->ctrl_info->ants[info->antid - FIRST_ANT_ID].row, info->ctrl_info->ants[info->antid - FIRST_ANT_ID].col);
	//LOGPID("Control: Moving to dir: %d\n", cmdreq->dir);
	//LOGPID("Control: Trying to move: %d, %d\n", nextPos.row, nextPos.col);
	if(nextPos.row >= info->ctrl_info->rows || nextPos.col >= info->ctrl_info->cols ||  nextPos.row < 0 || nextPos.col < 0){
		info->ctrl_info->ants[info->antid - FIRST_ANT_ID].status = ANT_DECIDED;
		info->ctrl_info->ants[info->antid - FIRST_ANT_ID].cmd = newMoveRes(STATUS_FAILED);
		LOGPID("Control: Move Failed\n\n");
		return NULL;
	}
	
	if(info->ctrl_info->board[nextPos.row][nextPos.col].obj != NO_OBJ &&
			info->ctrl_info->board[nextPos.row][nextPos.col].obj != OBJ_ANT){
		info->ctrl_info->ants[info->antid - FIRST_ANT_ID].status = ANT_DECIDED;
		info->ctrl_info->ants[info->antid - FIRST_ANT_ID].cmd = newMoveRes(STATUS_FAILED);
		LOGPID("Control: Move Failed\n\n");
		return NULL;
	}
	
	info->ctrl_info->ants[info->antid - FIRST_ANT_ID].status = ANT_MOVING;
	info->ctrl_info->ants[info->antid - FIRST_ANT_ID].dirpointing.row = nextPos.row;
	info->ctrl_info->ants[info->antid - FIRST_ANT_ID].dirpointing.col = nextPos.col;
	return NULL;
}


cmd_t ctrlHandleSmell(void * ptrInfo, cmd_t cmd){
	cmd_smell_req_t cmdreq = (cmd_smell_req_t) cmd;
	ant_and_ctrl_info_t info = (ant_and_ctrl_info_t) ptrInfo;
	
	LOGPID("Control: Handle CMD_SMELL, type: %d\n", cmdreq->type);
	
	struct st_dir_t currPos;
	currPos.row = info->ctrl_info->ants[info->antid - FIRST_ANT_ID].row;
	currPos.col = info->ctrl_info->ants[info->antid - FIRST_ANT_ID].col;
	
	tile_t tileRes = calloc(8, sizeof(struct tile_t));
	
	int mov[16] = {-1,0, -1,1 , 0,1, 1,1, 1,0, 1,-1, 0,-1, -1,-1};
	
	int i;
	for(i = 0; i < 16; i = i + 2){
		if(currPos.row + mov[i] < info->ctrl_info->rows && currPos.col + mov[i+1] < info->ctrl_info->cols &&
				currPos.row + mov[i] >= 0 && currPos.col + mov[i+1] >= 0){
			if(thereIsAnAnt(info->ctrl_info, currPos.row + mov[i], currPos.col + mov[i+1]) ){
				tileRes[i/2].obj = OBJ_ANT;
			}else{
				tileRes[i/2].obj = info->ctrl_info->board[currPos.row + mov[i]][currPos.col + mov[i+1]].obj;
				tileRes[i/2].trail = info->ctrl_info->board[currPos.row + mov[i]][currPos.col + mov[i+1]].trail;
			}
		}else{
			tileRes[i/2].obj = OBJ_OUT_OF_BOUNDS;
		}
		LOGPID("Control: OBJ(%d)=%d\n", i/2, tileRes[i/2].obj);
	}
	
	info->ctrl_info->ants[info->antid - FIRST_ANT_ID].cmd = newSmellRes(tileRes);
	info->ctrl_info->ants[info->antid - FIRST_ANT_ID].status = ANT_DECIDED;
	return NULL;
}


cmd_t ctrlHandlePick(void * ptrInfo, cmd_t cmd){
	cmd_pick_req_t cmdreq = (cmd_pick_req_t) cmd;
	ant_and_ctrl_info_t info = (ant_and_ctrl_info_t) ptrInfo;
	
	LOGPID("Control: Handle CMD_PICK, type: %d\n", cmdreq->type);
	
	struct st_dir_t currPos;
	currPos.row = info->ctrl_info->ants[info->antid - FIRST_ANT_ID].row;
	currPos.col = info->ctrl_info->ants[info->antid - FIRST_ANT_ID].col;
	
	int mov[8] = {-1,0, 0,1, 1,0, 0,-1};
	
	struct st_dir_t foodPos;
	foodPos.row = info->ctrl_info->ants[info->antid - FIRST_ANT_ID].row + mov[cmdreq->dir];
	foodPos.col = info->ctrl_info->ants[info->antid - FIRST_ANT_ID].col + mov[cmdreq->dir+1];
	
	if(foodPos.row >= info->ctrl_info->rows || foodPos.col >= info->ctrl_info->cols ||
			foodPos.row < 0 || foodPos.col < 0 || info->ctrl_info->ants[info->antid - FIRST_ANT_ID].carrying != NO_OBJ){
		info->ctrl_info->ants[info->antid - FIRST_ANT_ID].status = ANT_DECIDED;
		info->ctrl_info->ants[info->antid - FIRST_ANT_ID].cmd = newPickRes(STATUS_FAILED);
		return NULL;
	}
	
	if(info->ctrl_info->board[foodPos.row][foodPos.col].obj == OBJ_FOOD){
		info->ctrl_info->board[foodPos.row][foodPos.col].obj = NO_OBJ;
		info->ctrl_info->ants[info->antid - FIRST_ANT_ID].carrying = OBJ_FOOD;
		info->ctrl_info->ants[info->antid - FIRST_ANT_ID].cmd = newPickRes(STATUS_OK);
		info->ctrl_info->ants[info->antid - FIRST_ANT_ID].status = ANT_DECIDED;
		LOGPID("Control: LEVANTO COMIDA! status: %d\n", info->ctrl_info->ants[info->antid - FIRST_ANT_ID].status);
		return NULL;
	}
	if(info->ctrl_info->board[foodPos.row][foodPos.col].obj == OBJ_BIGFOOD){
		info->ctrl_info->ants[info->antid - FIRST_ANT_ID].dirpointing.row = foodPos.row;
		info->ctrl_info->ants[info->antid - FIRST_ANT_ID].dirpointing.col = foodPos.col;
		LOGPID("Control: COMIDA GRANDE!\n");
		info->ctrl_info->ants[info->antid - FIRST_ANT_ID].status = ANT_NEEDHELP;
		return NULL;
	}
	info->ctrl_info->ants[info->antid - FIRST_ANT_ID].status = ANT_DECIDED;
	info->ctrl_info->ants[info->antid - FIRST_ANT_ID].cmd = newPickRes(STATUS_FAILED);
	return NULL;
}


cmd_t ctrlHandleYell(void * ptrInfo, cmd_t cmd){
	cmd_yell_req_t cmdreq = (cmd_yell_req_t) cmd;
	ant_and_ctrl_info_t info = (ant_and_ctrl_info_t) ptrInfo;
	
	LOGPID("Control: Handle CMD_PICK, type: %d\n", cmdreq->type);
	
	info->ctrl_info->ants[info->antid - FIRST_ANT_ID].yelled = 1;
	
	cmd_t cmdres = newYellRes();
	info->ctrl_info->ants[info->antid - FIRST_ANT_ID].status = ANT_DECIDED;
	return NULL;
}

cmd_t ctrlHandleStop(void * ptrInfo, cmd_t cmd){
	ant_and_ctrl_info_t info = (ant_and_ctrl_info_t) ptrInfo;
	info->ctrl_info->ants[info->antid - FIRST_ANT_ID].status = ANT_STOPPED;
	
	LOGPID("Control: Handle CMD_STOP, type: %d\n", cmd->type);
	
	return NULL;
}
