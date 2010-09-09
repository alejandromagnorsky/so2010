#include "../include/io.h"

int cargarTablero(tablero_t tab, char * nombreArchivo)
{
	int c, aux;
	int i,k;
	
	FILE * archivo;

	archivo = fopen(nombreArchivo,"r");
	
	if(archivo == NULL) 
		return ERR_FILE;
		
	c = 0;
	fread(&c,sizeof(char),1,archivo);
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
		fread(&c,sizeof(char),1,archivo);
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
	
	tab->tabCols = aux;
	printf("cant cols: %d\n",aux);
	aux = 0;
	
	c = 0;
	fread(&c,sizeof(char),1,archivo);
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
		fread(&c,sizeof(char),1,archivo);
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
	
	tab->tabFils = aux;
	printf("cant fils: %d\n",aux);
	aux = 0;
	
	c = 0;
	fread(&c,sizeof(char),1,archivo);
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
		fread(&c,sizeof(char),1,archivo);
		c -= 48;
		if(c > 0 && c <= 9)
		{
			aux = aux * 10 + c;
		}
	}
	
	if(aux >= tab->tabCols)
	{
		return ERR_HORMIGUERO;
	}
	
	tab->horCol = aux;
	printf("column hormiguero: %d\n",aux);
	aux = 0;
	
	c = 0;
	fread(&c,sizeof(char),1,archivo);
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
		fread(&c,sizeof(char),1,archivo);
		c -= 48;
		if(c > 0 && c <= 9)
		{
			aux = aux * 10 + c;
		}
	}
	
	if(aux >= tab->tabFils)
	{
		return ERR_HORMIGUERO;
	}
	
	tab->horFil = aux;
	printf("filas hormiguero: %d\n",aux);
	aux = 0;
	
	c = 0;
	fread(&c,sizeof(char),1,archivo);
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
		fread(&c,sizeof(char),1,archivo);
		c -= 48;
		if(c > 0 && c <= 9)
		{
			aux = aux * 10 + c;
		}
	}
	
	//[TODO] verificar si hay un maximo de cantidad de hormigas
	/*if(aux >= tab->tabFils)
	{
		return ERR_HORMIGUERO;
	}*/
	
	tab->cantHor = aux;
	printf("cant hormigas: %d\n",aux);
	aux = 0;
	
	c = 0;
	fread(&c,sizeof(char),1,archivo);
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
		fread(&c,sizeof(char),1,archivo);
		c -= 48;
		if(c > 0 && c <= 9)
		{
			aux = aux * 10 + c;
		}
	}
	
	if(aux >= tab->tabFils * tab->tabCols - 1)
	{
		return ERR_CANTCOMCHICA;
	}
	
	tab->cantComChicas = aux;
	printf("cant com chica: %d\n",aux);
	aux = 0;
	
	c = alocarComidasChicas(tab, tab->cantComChicas);
	
	for(i = 0; i < tab->cantComChicas; i++)
	{
		
	}
	
	/*c = 0;
	fread(&c,sizeof(char),1,archivo);
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
		fread(&c,sizeof(char),1,archivo);
		c -= 48;
		if(c > 0 && c <= 9)
		{
			aux = aux * 10 + c;
		}
	}
	
	if(aux >= tab->tabFils * tab->tabCols - 1 + tab->cantComChicas)
	{
		return ERR_CANTCOMCHICA;
	}
	
	tab->cantComGrandes = aux;
	printf("cant com grande: %d\n",aux);
	aux = 0;*/
	
	
	//c = alocarComidas(tab, tab->cantComChicas, tab-> cantComGrandes)
}


int alocarComidasChicas(tablero_t tab, int cantComChicas)
{
	tablero_t->cantComChicas[0] = malloc(sizeof(int)*cantComChicas*2);
}

int alocarComidasGrandes(tablero_t tab, int cantComGrandes)
{
	tablero_t->cantComChicas[0] = malloc(sizeof(int)*cantComChicas*2);
}

tablero_t tnew()
{
	tablero_t tab = (tablero_t) malloc(sizeof(struct st_tablero_t));
	
	if(tab)
	{
		return tab;
	}
	else
	{
		return NULL;
	}
}
