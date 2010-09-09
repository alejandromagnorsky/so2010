#ifndef _parser_
#define _parser_

#include <stdio.h>
#include <stdlib.h>

#define MAX_FILS 80
#define MAX_COLS 80

struct st_tablero_t {
    int tabCols;
    int tabFils;
    
    int horCol;
    int horFil;
    
    int cantHor;
    
    int cantComChicas;
    int cantComGrandes;
    
    int ** comChicas;
    int ** comGrandes;
};

typedef struct st_tablero_t* tablero_t;


enum {
    ERR_FILE,
    MEMORY_ERROR,
    ERR_HORMIGUERO,
    ERR_CANTCOMCHICA,
    ERR_CANTCOMTOTAL
};


int cargarTablero(tablero_t tab, char * nombreArchivo);

tablero_t tnew();

int alocarComidasChicas(tablero_t tab, int cantComChicas);

int alocarComidasGrandes(tablero_t tab, int cantComGrandes);

#endif
