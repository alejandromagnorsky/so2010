#include "../include/io.h"

int loadGrid(grid_t grid, char * nameFile)
{
	int c, aux, i;
	
	FILE * file;

	file = fopen(nameFile,"r");
	
	if(file == NULL) 
		return ERR_FILE;
		
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
	
	//[TODO] chequear espacios
	
	c = 0;
	fread(&c,sizeof(char),1,file);
	c -= 48;
	if(c > 0 && c <= 9)
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
	
	c = 0;
	fread(&c,sizeof(char),1,file);
	c -= 48;
	if(c > 0 && c <= 9)
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
	
	//[TODO] chequear espacios
	
	c = 0;
	fread(&c,sizeof(char),1,file);
	c -= 48;
	if(c > 0 && c <= 9)
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
	
	//[TODO] chequear espacios
	
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
	
	if(aux >= grid->gridRows * grid->gridCols - 1)
	{
		return ERR_SMALLFOODQUANT;
	}
	
	grid->smallFoodQuant = aux;
	aux = 0;
	
	//[TODO] chequear espacios
	
	c = allocSmallFood(grid, grid->smallFoodQuant);

	if(c == MEMORY_ERROR)
	{
		return MEMORY_ERROR;
	}

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
	
		if(aux >= grid->gridRows * grid->gridCols - 1)
		{
			return ERR_SMALLFOODQUANT;
		}
	
		grid->smallFoods[i] = aux;
		aux = 0;
	}
	
	//[TODO] chequear espacios
	
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
	
	if(aux >= grid->gridRows * grid->gridCols - 1 + grid->smallFoodQuant)
	{
		return ERR_BIGFOODQUANT;
	}
	
	grid->bigFoodQuant = aux;
	aux = 0;
	
	c = allocBigFood(grid, grid->bigFoodQuant);

	if(c == MEMORY_ERROR)
	{
		return MEMORY_ERROR;
	}
	
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
	
		if(aux >= grid->gridRows * grid->gridCols - 1)
		{
			return ERR_BIGFOODQUANT;
		}
	
		grid->bigFoods[i] = aux;
		aux = 0;
	}
	
	fclose(file);
	
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
