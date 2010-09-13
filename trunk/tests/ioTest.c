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
	
	
	grid = gnew();
	
	if(k = loadGrid(grid, "ejemploTablero") != NO_ERRORS)
	{
		printf("ERROR CARGANDO GRILLA: %d\n",k);
		return 0;
	}
	
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
	
	
	board[2][0].obj = 8;
	board[2][0].trail = 0;
	board[2][1].obj = 8;
	board[2][1].trail = 0.1;
	board[2][2].obj = 8;
	board[2][2].trail = 0.2;
	board[2][3].obj = 8;
	board[2][3].trail = 0.3;
	board[2][4].obj = 8;
	board[2][4].trail = 0.4;
	board[2][5].obj = 8;
	board[2][5].trail = 0.5;
	board[2][6].obj = 8;
	board[2][6].trail = 0.6;
	board[2][7].obj = 8;
	board[2][7].trail = 0.7;
	board[2][8].obj = 8;
	board[2][8].trail = 0.8;
	board[2][9].obj = 8;
	board[2][9].trail = 0.9;
	board[2][10].obj = 8;
	board[2][10].trail = 1;
	
	board[1][3].obj = OBJ_ANT;
	board[1][4].obj = OBJ_ANTHILL;
	board[1][5].obj = OBJ_FOOD;
	board[1][6].obj = OBJ_BIGFOOD;
	board[1][7].obj = NO_OBJ;
	
	//board[14][78].obj = OBJ_ANT;
	
	
	
	for(i = 0; i < grid->gridRows; i++)
	{
		for(j = 0; j < grid->gridCols; j++)
		{
			if(board[i][j].obj == NO_OBJ)
			{
				addCharAt(i,j,' ');
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
	
	for(i = 0; i < grid->gridRows; i++){
		free(board[i]);
	}
	free(board);
	
	freeGrid(grid);
	
	
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

