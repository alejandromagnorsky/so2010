#ifndef	_SHELL_H_
#define	_SHELL_H_

#include "klib.h"
#include "keyboard.h"

#define ROWS 25
#define COLS 80
#define VIDEO_SIZE (ROWS * COLS * 2)
#define LAST_ROW VIDEO_SIZE - 2 * COLS

typedef struct {
	char address[VIDEO_SIZE];
	size_t wpos;
	size_t rpos;
} screen_t;


/*
struct device_t {
    int  id;
    char name[11];
    
    driver_t driver;

    void* addr;    
    size_t size;    
    
    size_t wpos;    
    size_t rpos;	
};
*/

size_t video_write(device_t dev, void* from, size_t nbytes);
void move_cursor(int position);
void move_cursor_rc(int row, int col);
void scroll(device_t dev);


extern struct system_t System;
#endif
