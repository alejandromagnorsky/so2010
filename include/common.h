#ifndef _common_
#define _common_

struct st_dir_info{
	int row;
	int col;
};

typedef struct st_dir_info * dir_info_t;

typedef struct tile_t ** board_t;

#endif
