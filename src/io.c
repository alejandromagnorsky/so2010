#include "../include/io.h"

int loadGrid(grid_t grid, char * fileName)
{
	int c, aux, i;
	
	FILE * file;

	file = fopen(fileName,"r");
	
	if(file == NULL) 
		return ERR_FILE;
	
	/* Reading quantity of columns */	
	c = 0;
	fread(&c,sizeof(char),1,file);
	c -= 48;
	if(c >= 0 && c <= 9)
	{
		aux = c;
	}
	else 
	{
		return ERR_FILE;
	}
	
	while(c >= 0 && c <= 9)
	{
		fread(&c,sizeof(char),1,file);
		c -= 48;
		if(c >= 0 && c <= 9)
		{
			aux = aux * 10 + c;
		}
	}
	
	if(aux > MAX_COLS)
	{
		return ERR_FILE;
	}
	
	grid->gridCols = aux;
	aux = 0;
	
	if(c != -4)
	{
		return ERR_FILE;
	}
	
	/* Reading quantity of rows */

	c = 0;
	fread(&c,sizeof(char),1,file);
	c -=48;
	if(c >= 0 && c <= 9)
	{
		aux = c;
	}
	else 
	{
		return ERR_FILE;
	}
	
	while(c >= 0 && c <= 9)
	{
		fread(&c,sizeof(char),1,file);
		c -= 48;
		if(c >= 0 && c <= 9)
		{
			aux = aux * 10 + c;
		}
	}
	
	if(aux > MAX_FILS)
	{
		return ERR_FILE;
	}
	
	grid->gridRows = aux;
	aux = 0;
	
	if((grid->gridRows * grid->gridCols) < 2)
	{
		return ERR_FILE;
	}
	
	/* Reading anthill column */
	
	c = 0;
	fread(&c,sizeof(char),1,file);
	c -= 48;
	if(c >= 0 && c <= 9)
	{
		aux = c;
	}
	else 
	{
		return ERR_FILE;
	}
	
	while(c >= 0 && c <= 9)
	{
		fread(&c,sizeof(char),1,file);
		c -= 48;
		if(c >= 0 && c <= 9)
		{
			aux = aux * 10 + c;
		}
	}
	
	if(aux >= grid->gridCols)
	{
		return ERR_ANTHILL;
	}
	
	grid->anthillCol = aux;
	aux = 0;
	
	if(c != -4)
	{
		return ERR_FILE;
	}
	
	/* Reading anthill row */
	
	c = 0;
	fread(&c,sizeof(char),1,file);
	c -= 48;
	if(c >= 0 && c <= 9)
	{
		aux = c;
	}
	else 
	{
		return ERR_FILE;
	}
	
	while(c >= 0 && c <= 9)
	{
		fread(&c,sizeof(char),1,file);
		c -= 48;
		if(c >= 0 && c <= 9)
		{
			aux = aux * 10 + c;
		}
	}
	
	if(aux >= grid->gridRows)
	{
		return ERR_ANTHILL;
	}
	
	grid->anthillRow = aux;
	aux = 0;
	
	/* Reading quantity of ants */
	
	c = 0;
	fread(&c,sizeof(char),1,file);
	c -= 48;
	if(c >= 0 && c <= 9)
	{
		aux = c;
	}
	else 
	{
		return ERR_FILE;
	}
	
	while(c >= 0 && c <= 9)
	{
		fread(&c,sizeof(char),1,file);
		c -= 48;
		if(c >= 0 && c <= 9)
		{
			aux = aux * 10 + c;
		}
	}

	if(aux < 0)
	{
		return ERR_FILE;
	}	
	
	grid->antsQuant = aux;
	aux = 0;
	
	/* Reading quantity of small food */
	
	c = 0;
	fread(&c,sizeof(char),1,file);
	c -= 48;
	if(c >= 0 && c <= 9)
	{
		aux = c;
	}
	else 
	{
		return ERR_FILE;
	}
	
	while(c >= 0 && c <= 9)
	{
		fread(&c,sizeof(char),1,file);
		c -= 48;
		if(c >= 0 && c <= 9)
		{
			aux = aux * 10 + c;
		}
	}
	
	grid->smallFoodQuant = aux;
	aux = 0;
	
	c = allocSmallFood(grid, grid->smallFoodQuant);

	if(c == MEMORY_ERROR)
	{
		return MEMORY_ERROR;
	}
	
	/* Reading small food positions */

	for(i = 0; i < grid->smallFoodQuant*2; i++)
	{
		c = 0;
		fread(&c,sizeof(char),1,file);
		c -= 48;
		if(c >= 0 && c <= 9)
		{
			aux = c;
		}
		else 
		{
			return ERR_FILE;
		}
	
		while(c >= 0 && c <= 9)
		{
			fread(&c,sizeof(char),1,file);
			c -= 48;
			if(c >= 0 && c <= 9)
			{
				aux = aux * 10 + c;
			}
		}
	
		grid->smallFoods[i] = aux;
		aux = 0;
	}
	
	/* Reading quantity of big food */
	
	c = 0;
	fread(&c,sizeof(char),1,file);
	c -= 48;
	if(c >= 0 && c <= 9)
	{
		aux = c;
	}
	else 
	{
		return ERR_FILE;
	}
	
	while(c >= 0 && c <= 9)
	{
		fread(&c,sizeof(char),1,file);
		c -= 48;
		if(c >= 0 && c <= 9)
		{
			aux = aux * 10 + c;
		}
	}
	
	grid->bigFoodQuant = aux;
	aux = 0;
	
	c = allocBigFood(grid, grid->bigFoodQuant);

	if(c == MEMORY_ERROR)
	{
		return MEMORY_ERROR;
	}
	
	/* Reading big food positions */
	
	for(i = 0; i < grid->bigFoodQuant*2; i++)
	{
		c = 0;
		fread(&c,sizeof(char),1,file);
		c -= 48;
		if(c >= 0 && c <= 9)
		{
			aux = c;
		}
		else 
		{
			return ERR_FILE;
		}
	
		while(c >= 0 && c <= 9)
		{
			fread(&c,sizeof(char),1,file);
			c -= 48;
			if(c >= 0 && c <= 9)
			{
				aux = aux * 10 + c;
			}
		}
	
		grid->bigFoods[i] = aux;
		aux = 0;
	}
	
	fclose(file);
	
	if(checkFoodPositions(grid) == ERR_FOODPOSITION)
	{
		return ERR_FOODPOSITION;
	}
	
	return NO_ERRORS;
}


int allocSmallFood(grid_t grid, int smallFoodQuant)
{
	grid->smallFoods = (int *)malloc(sizeof(int)*smallFoodQuant*2);
	if(grid->smallFoods)
	{
		return NO_ERRORS;
	}
	else
	{
		return MEMORY_ERROR;
	}
}

int allocBigFood(grid_t grid, int bigFoodQuant)
{
	grid->bigFoods = (int *)malloc(sizeof(int)*bigFoodQuant*2);
	if(grid->bigFoods)
	{
		return NO_ERRORS;
	}
	else
	{
		return MEMORY_ERROR;
	}
}

grid_t gnew()
{
	grid_t grid = (grid_t) malloc(sizeof(struct st_grid_t));
	
	if(grid)
	{
		return grid;
	}
	else
	{
		return NULL;
	}
}

int checkFoodPositions(grid_t grid)
{
	int i;
	//[TODO] chequear comida superpuesta
	//int ** board = calloc(grid->gridRows, sizeof(int));
	
	for(i = 0; i < grid->smallFoodQuant*2; i++)
	{
		if(i%2 == 0)
		{
			if(grid->smallFoods[i] >= grid->gridCols)
			{
				return ERR_FOODPOSITION;
			}
		}
		else
		{
			if(grid->smallFoods[i] >= grid->gridRows)
			{
				return ERR_FOODPOSITION;
			}
		}
	}
	
	for(i = 0; i < grid->bigFoodQuant*2; i++)
	{
		if(i%2 == 0)
		{
			if(grid->bigFoods[i] >= grid->gridCols)
			{
				return ERR_FOODPOSITION;
			}
		}
		else
		{
			if(grid->bigFoods[i] >= grid->gridRows)
			{
				return ERR_FOODPOSITION;
			}
		}
	}
	
	return NO_ERRORS;
}

int initializeScreen(grid_t grid)
{
	int i;
	double num;
	char * msg;

	initscr();
	noecho();
	
	if(has_colors() == FALSE)
	{	endwin();
		printf("Your terminal does not support color\n");
		exit(1);
	}
	else
	{
		start_color();
		init_pair(TEMPPAIR, COLOR_WHITE, COLOR_BLACK);
		attrset(COLOR_PAIR(TEMPPAIR));
	}

	for(i = 0; i < grid->gridRows + 1; i++)
	{
		mvaddch(i, 0, '|');
	}
	
	for(i = 0; i < grid->gridRows + 1; i++)
	{
		mvaddch(i, grid->gridCols * 5 + 1, '|');
	}
	
	for(i = 0; i < grid->gridCols * 5 + 1; i++)
	{
		mvaddch(0, i, '-');
	}
	
	for(i = 0; i < grid->gridCols * 5 + 1; i++)
	{
		mvaddch(grid->gridRows + 1, i, '-');
	}
	
	mvaddch(0, 0, '*');
	mvaddch(grid->gridRows + 1, grid->gridCols * 5 + 1, '*');
	mvaddch(0, grid->gridCols * 5 + 1, '*');
	mvaddch(grid->gridRows + 1, 0, '*');
	
	msg = "| @ = ant | H = anthill | s = small food | B = big food |";
	addStringAt(grid->gridRows + 4, 0, msg);
	
	
	/*addCharAt(grid->anthillRow, grid->anthillCol, 'H');
	
	for(i = 0; i < grid->smallFoodQuant * 2; i+=2)
	{
		addCharAt(grid->smallFoods[i+1], grid->smallFoods[i], 's');
	}
	
	for(i = 0; i < grid->bigFoodQuant * 2; i+=2)
	{
		addCharAt(grid->bigFoods[i+1], grid->bigFoods[i], 'B');
	}*/
	// [TODO] sacar el endwin y el getch en caso que sea necesario
	
	attroff(COLOR_PAIR(TEMPPAIR));
	getch();
	refresh();
	/*endwin();*/
}

void addCharAt(int col, int row, char c)
{
	mvaddch(col + 1, row * 5 + 2, c);
}

void addDoubleAt(int col, int row, double num)
{
	int color = getColor(lround(num * 5));
	init_pair(color, COLOR_WHITE, color);
	
	move(col + 1, row * 5 + 1);
	
	attrset(COLOR_PAIR(color));
	printw("%g",num);
	attroff(COLOR_PAIR(color));

}

void addStringAt(int col, int row, char * str)
{
	move(col, row);
	printw("%s", str);
}

void refreshGrid(board_t board, grid_t grid)
{
	int i,j;
	
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
	getch();
	refresh();
}

int getColor(int color)
{
	switch(color)
	{
		case 0:
			return 3;
			break;
		case 1:
			return 1;
			break;
		case 2:
			return 5;
			break;
		case 3:
			return 6;
			break;
		case 4:
			return 4;
			break;
		case 5:
			return 4;
			break;
		default:
			return 0;
	}
}
