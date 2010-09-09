#include "../include/io.h"

int main()
{
	int i;

	grid_t grid = gnew();
	
	if(loadGrid(grid, "ejemploTablero") == NO_ERRORS)
	{
	
		printf("Quantity of columns: %d\n", grid->gridCols);
		printf("Quantity of rows: %d\n", grid->gridRows);
	
		printf("Anthill column: %d\n", grid->anthillCol);
		printf("Anthill row: %d\n", grid->anthillRow);
	
		printf("Quantity of ants: %d\n", grid->antsQuant);
	
		printf("Quantity of small foods: %d\n", grid->smallFoodQuant);
		printf("Quantity of big foods: %d\n", grid->bigFoodQuant);
	
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
	else
	{
		printf("ERROR IN CONFIGURATION FILE\n");
	}
	
	
}

