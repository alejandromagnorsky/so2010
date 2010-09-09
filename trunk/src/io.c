#include "../include/io.h"

int loadGrid(grid_t grid, char * nameFile)
{
	int c, aux;
	int i,k;
	
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
		if(c > 0 && c <= 9)
		{
			aux = aux * 10 + c;
		}
	}
	
	if(aux > MAX_COLS)
	{
		return ERR_FILE;
	}
	
	grid->gridCols = aux;
	printf("cant cols: %d\n",aux);
	aux = 0;
	
	if(c != -4)
	{
		return ERR_FILE;
	}
	else 
	{
		printf("asdfasdf\n");
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
		if(c > 0 && c <= 9)
		{
			aux = aux * 10 + c;
		}
	}
	
	if(aux > MAX_FILS)
	{
		return ERR_FILE;
	}
	
	grid->gridRows = aux;
	printf("cant fils: %d\n",aux);
	aux = 0;
	
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
		if(c > 0 && c <= 9)
		{
			aux = aux * 10 + c;
		}
	}
	
	if(aux >= grid->gridCols)
	{
		return ERR_HORMIGUERO;
	}
	
	grid->antCol = aux;
	printf("column hormiguero: %d\n",aux);
	aux = 0;
	
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
		if(c > 0 && c <= 9)
		{
			aux = aux * 10 + c;
		}
	}
	
	if(aux >= grid->gridRows)
	{
		return ERR_HORMIGUERO;
	}
	
	grid->antRow = aux;
	printf("filas hormiguero: %d\n",aux);
	aux = 0;
	
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
		if(c > 0 && c <= 9)
		{
			aux = aux * 10 + c;
		}
	}
	
	//[TODO] verificar si hay un maximo de cantidad de hormigas
	/*if(aux >= grid->gridRows)
	{
		return ERR_HORMIGUERO;
	}*/
	
	grid->cantHor = aux;
	printf("cant hormigas: %d\n",aux);
	aux = 0;
	
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
		if(c > 0 && c <= 9)
		{
			aux = aux * 10 + c;
		}
	}
	
	if(aux >= grid->gridRows * grid->gridCols - 1)
	{
		return ERR_CANTCOMCHICA;
	}
	
	grid->smallFoodQuant = aux;
	printf("cant com chica: %d\n",aux);
	aux = 0;
	
	c = alocarComidasChicas(grid, grid->smallFoodQuant);
	
	for(i = 0; i < grid->smallFoodQuant; i++)
	{
		
	}
	
	/*c = 0;
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
		if(c > 0 && c <= 9)
		{
			aux = aux * 10 + c;
		}
	}
	
	if(aux >= grid->gridRows * grid->gridCols - 1 + grid->smallFoodQuant)
	{
		return ERR_CANTCOMCHICA;
	}
	
	grid->bigFoodQuant = aux;
	printf("cant com grande: %d\n",aux);
	aux = 0;*/
	
	
	//c = alocarComidas(grid, grid->smallFoodQuant, grid-> bigFoodQuant)
}


int alocarComidasChicas(grid_t grid, int smallFoodQuant)
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

int alocarComidasGrandes(grid_t grid, int bigFoodQuant)
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
