#ifndef _IO_H_
#define _IO_H_

#include "klib.h"
#include "screen.h"

/************************************ 
**
** I/O library header file
**
*************************************/

/************************************************
** I/O primitives (should NOT be used directly):
*************************************************/

void _memcpy(void* from, void* to, size_t nbytes);

unsigned char _inport(unsigned char port);
void _outport(unsigned char port, unsigned char c);


/************************************************
** Device I/O functions:
*************************************************/

/* Driver abstraction layer */
/* agregado para SO, es un wrapper medio mentiroso */
size_t _kb_write(int devcode, void* from, size_t nbytes);
size_t _kb_read(int devcode, void* to, size_t nbytes);

size_t _screen_write(int devcode, void* from, size_t nbytes);

/* Generic direct interaction: */
device_t _dget(system_t, int index);
size_t _dread(device_t, void* to, size_t nbytes);
size_t _dwrite(device_t, void* from, size_t nbytes);
size_t _dtellr(device_t dev);
size_t _dtellw(device_t dev);
size_t _dseekw(device_t, int offset, int from);
size_t _dseekr(device_t, int offset, int from);

/* Linear reading/writing: */
size_t _dputb(device_t, char);

/************************************************
** Higher level IO functions:
*************************************************/
void putchar(char c);
void puts(char* str);
void putd(int d);
void putu(unsigned int u);
void printf(char* fstr, ...);

unsigned char getchar();
int sscanf(char* str, char* fmt, ...);
int scanf(char* fstr, ...);

extern struct system_t System;

#endif
