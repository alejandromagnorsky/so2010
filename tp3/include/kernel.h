#ifndef _KERNEL_H_
#define _KERNEL_H_

#include "kasm.h"
#include "defs.h"
#include "klib.h"
#include "string.h"
#include "io.h"
#include "keyboard.h"
#include "shell.h"
#include "screen.h"
#include "mem.h"
#include "tty.h"
#include "multiboot.h"



/********************************** 
**
** Kernel header file
**
***********************************/
void initializeIDT();
void update_pointer(int row, int col);
/************************************************
** Constants and definitions:
*************************************************/

#define OS_PID	0

#define REFRESH_RATE 1

typedef enum eINT_80 {WRITE=0, READ} tINT_80;
typedef enum eUSER {U_KERNEL=0, U_NORMAL} tUSERS;

enum{
	NO_CMD = 0,
	CMD
};

/************************************************
** Macros for system functions:
*************************************************/

void _sys_addTick();
long int _sys_getTicks();


/************************************************
** External namespaces:
*************************************************/
extern struct driver_t KeyboardDriver;
extern struct driver_t ScreenDriver;
extern struct driver_t TtyDriver;
extern struct PagingNamespace Paging;
extern struct TaskNamespace Task;
extern struct TTYSNamespace TTYS;
extern struct KBNamespace Keyboard;
extern struct TopNamespace Top;
extern struct disk_t Disk;

#endif
