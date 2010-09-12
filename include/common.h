#ifndef _COMMON_H_
#define _COMMON_H_

typedef struct tile_t ** board_t;

struct st_dir_t{
	int row;
	int col;
};

typedef struct st_dir_info * dir_info_t;

struct st_grid_t {
    int gridCols;
    int gridRows;
    
    int anthillCol;
    int anthillRow;
    
    int antsQuant;
    
    int smallFoodQuant;
    int bigFoodQuant;
    
    int * smallFoods;
    int * bigFoods;
};

typedef struct st_grid_t* grid_t;

#endif
