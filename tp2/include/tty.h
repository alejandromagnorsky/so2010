#ifndef _TTY_H_
#define _TTY_H_

#include "screen.h"
#include "keyboard.h"

#define NTTYS 4

#define ROWS 25
#define COLS 80
#define VIDEO_SIZE (ROWS * COLS * 2)
#define LAST_ROW VIDEO_SIZE - 2 * COLS

enum {
	TTY_READY = 0,
	TTY_WRITTEN
};

typedef struct{
	unsigned char shift_status;
	unsigned char mayus_status;
	unsigned char num_status;
	unsigned char scroll_status;
	unsigned char accent_status;
	unsigned char escaped;
}flags_t;

typedef struct {
	char address[VIDEO_SIZE];
	size_t wpos;
	size_t rpos;
} screen_t;

typedef struct {
	flags_t flags;
}input_t;

typedef screen_t output_t;

typedef struct {
	int status;
	input_t input;
	output_t output;
}tty_t;

void _initialize_ttys();
void _refresh();
void _save();

extern tty_t ttys[NTTYS];
extern struct system_t System;
extern struct KBNamespace Keyboard;

struct TTYSNamespace {
	void (*initialize)();
	void (*refresh)();
	void (*save)();
	void (*load)();
	void (*jumpToTTY)(int);
	void (*createTTYs)();
};

int _createTty1(char * a);
int _createTty2(char * a);
int _createTty3(char * a);
int _createTty4(char * a);

#endif
