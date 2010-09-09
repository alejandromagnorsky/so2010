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
	if(c >= 2 && c <= 9)
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
	if(c >= 2 && c <= 9)
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
}

