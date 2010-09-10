#include "../include/server.h"

int main(int argc, char * argv[]){
	
	if(argc != 2){
		printf("Error: Needs a filename.\n");
		exit(1);
	}
	
	char * filename = argv[1];
	
	grid_t gridinfo;
	if((gridinfo = getGridInfo(filename)) == NULL){
		printf("Error: File problem.\n");
		exit(1);
	}
	
	board_t board;
	int rows = gridinfo->gridRows, cols = gridinfo->gridCols;
	if((board = createBoard(rows, cols)) == NULL){
		printf("Error: Memory error.\n");
		exit(1);
	}
	
	int qtyAnt = gridinfo->antsQuant;
	struct st_dir_info* ants = createAntPosArray(qtyAnt, gridinfo);
	
	fillWithFood(board, gridinfo);
	
	int turn = 0;
	
	int i, j;
	
	for(i = 0; i < qtyAnt; i++){
		board[ants[i].row][ants[i].col].obj = OBJ_ANT;
	}
	
	for(i = 0; i < rows; i++){
		for(j = 0; j < cols; j++){
			if(board[i][j].obj == NO_OBJ){
				printf("    |");
			}else if(board[i][j].obj == OBJ_FOOD){
				printf("  C |");
			}else if(board[i][j].obj == OBJ_BIGFOOD){
				printf("  B |");
			}else{
				printf(" %2d |", board[i][j].obj, board[i][j].trail);
			}
		}
		printf("\n");
	}
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

struct st_dir_info* createAntPosArray(int qtyAnt, grid_t gridinfo){
	struct st_dir_info* ants = calloc(qtyAnt, sizeof(struct st_dir_info));
	int i = 0;
	for(i = 0; i < qtyAnt; i++){
		ants[i].row = gridinfo->anthillRow;
		ants[i].col = gridinfo->anthillCol;
	}
	return ants;
}

grid_t getGridInfo(char * filename){
	grid_t gridinfo = (grid_t) malloc(sizeof(struct st_grid_t));
	if(gridinfo == NULL){
		return NULL;
	}
	if(loadGrid(gridinfo, filename) != NO_ERRORS){
		return NULL;
	}
	return gridinfo;
}

void fillWithFood(board_t board, grid_t gridinfo){
	int qtySmallFood = gridinfo->smallFoodQuant;
	int qtyBigFood = gridinfo->bigFoodQuant;
	int i;
	for(i = 0; i < qtySmallFood; i++){
		board[gridinfo->smallFoods[i]][gridinfo->smallFoods[i + 1]].obj = OBJ_FOOD;
	}
	for(i = 0; i < qtyBigFood; i++){
		board[gridinfo->bigFoods[i]][gridinfo->bigFoods[i + 1]].obj = OBJ_BIGFOOD;
	}
}