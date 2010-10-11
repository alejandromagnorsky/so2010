#ifndef	_SHELL_H_
#define	_SHELL_H_

#include "klib.h"
#include "keyboard.h"

#define WHITE_TXT 0x07 // Atributo de video. Letras blancas, fondo negro

/* Limpia la pantalla */
void k_clear_screen(void * addr);

size_t video_write(device_t dev, void* from, size_t nbytes);
void move_cursor(int position);
void move_cursor_rc(int row, int col);
void scroll(device_t dev);


extern struct system_t System;
#endif
