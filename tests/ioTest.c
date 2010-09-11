#include "../include/io.h"

int main()
{
	/* FILE TEST */

	/*int i, k;

	grid_t grid = gnew();
	
	k = loadGrid(grid, "ejemploTablero");
	
	if(k == NO_ERRORS)
	{
	
		printf("Quantity of columns: %d\n", grid->gridCols);
		printf("Quantity of rows: %d\n", grid->gridRows);
	
		printf("Anthill column: %d\n", grid->anthillCol);
		printf("Anthill row: %d\n", grid->anthillRow);
	
		printf("Quantity of ants: %d\n", grid->antsQuant);
	
		printf("Quantity of small food: %d\n", grid->smallFoodQuant);
		printf("Quantity of big food: %d\n", grid->bigFoodQuant);
	
		printf("small foods: ");
		for(i = 0; i < grid->smallFoodQuant*2; i++)
		{
			printf("%d", grid->smallFoods[i]);
			if(i%2 == 0)
			{
				printf(",");
			}
			else
			{
				printf("|");
			}
		}
		printf("\n");
	
		printf("big foods: ");
		for(i = 0; i < grid->bigFoodQuant*2; i++)
		{
			printf("%d", grid->bigFoods[i]);
			if(i%2 == 0)
			{
				printf(",");
			}
			else
			{
				printf("|");
			}
		}
		printf("\n");
	}
	else if(k == ERR_FOODPOSITION)
	{
		printf("ERROR IN FOOD POSITION\n");
	}
	else 
	{
		printf("ERROR IN CONFIGURATION FILE\n");
	}*/
	
	/* NCURSES TEST */
	
	int i,j,k, rows, cols;
	board_t board;
	grid_t grid;
	int qtyAnt;
	int qtySmallFood;
	int qtyBigFood;
	
	struct st_dir_info* ants = calloc(qtyAnt, sizeof(struct st_dir_info));
	
	grid = gnew();
	loadGrid(grid, "ejemploTablero");
	
	rows = grid->gridRows;
	cols = grid->gridCols;
	qtyAnt = grid->antsQuant;
	qtySmallFood = grid->smallFoodQuant;
	qtyBigFood = grid->bigFoodQuant;
	
	board = (board_t) malloc(sizeof(struct tile_t*) * rows);
	if(board == NULL){
		return -1;
	}
	
	for(i = 0; i < rows; i++){
		board[i] = (struct tile_t * ) malloc(sizeof(struct tile_t) * cols);
		if(board[i] == NULL){
			for(j = 0; j < i; j++){
			}
			return -1;
		}
		for(j = 0; j < cols; j++){
			board[i][j].obj = NO_OBJ;
			board[i][j].trail = 0;
		}
	}
	
	
	
	if(board == NULL){
		printf("Error: Memory error.\n");
		exit(1);
	}
	
	for(i = 0; i < qtyAnt; i++){
		ants[i].row = grid->anthillRow;
		ants[i].col = grid->anthillCol;
	}
	
	
	
	/*for(i = 0; i < qtySmallFood; i++){
		board[grid->smallFoods[i]][grid->smallFoods[i + 1]].obj = OBJ_FOOD;
	}
	for(i = 0; i < qtyBigFood*2; i++){
		board[grid->bigFoods[i]][grid->bigFoods[i + 1]].obj = OBJ_BIGFOOD;
	}*/
	
	
	if(k == NO_ERRORS)
	{
		k = initializeScreen(grid);
	}
	else 
	{
		printf("FILE ERROR\n");
	}
	
	
	board[0][0].obj = OBJ_FOOD;
	board[1][2].obj = OBJ_BIGFOOD;
	board[2][1].obj = OBJ_ANT;
	board[1][1].obj = OBJ_ANTHILL;
	
	for(i = 0; i < grid->gridRows; i++)
	{
		for(j = 0; j < grid->gridCols; j++)
		{
			if(board[i][j].obj == NO_OBJ)
			{
				addDoubleAt(i,j,0.05);
			}
			else if(board[i][j].obj == OBJ_FOOD)
			{
				addCharAt(i,j,'s');
			}
			else if(board[i][j].obj == OBJ_BIGFOOD)
			{
				addCharAt(i,j,'B');
			}
			else if(board[i][j].obj == OBJ_ANT)
			{
				addCharAt(i,j,'@');
			}
			else if(board[i][j].obj == OBJ_ANTHILL)
			{
				addCharAt(i,j,'H');
			}
			else
			{
				addDoubleAt(i,j,board[i][j].trail);
			}
		}
	}
	
	refresh();
	getch();
	endwin();
	
	
	
	/*for(i = 0; i < grid->gridRows; i++)
	{
		for(j = 0; j < grid->gridCols; j++)
		{
			if(board[i][j].obj == NO_OBJ)
			{
				printf(" ");
				//addCharAt(i,j,' ');
			}
			else if(board[i][j].obj == OBJ_FOOD)
			{
				printf("s");
				//addCharAt(i,j,'s');
			}
			else if(board[i][j].obj == OBJ_BIGFOOD)
			{
				printf("B");
				//addCharAt(i,j,'B');
			}
			else if(board[i][j].obj == OBJ_ANT)
			{
				printf("@");
				//addCharAt(i,j,'@');
			}
			else if(board[i][j].obj == OBJ_ANTHILL)
			{
				printf("H");
				//addCharAt(i,j,'H');
			}
			else
			{
				printf("M");
				//saddDoubleAt(i,j,board[i][j].trail);
			}
		}
	}*/
}

