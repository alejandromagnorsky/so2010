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
#define BUFFER_SIZE 1000

typedef enum eINT_80 {WRITE=0, READ} tINT_80;
typedef enum eUSER {U_KERNEL=0, U_NORMAL} tUSERS;

/************************************************
** Macros for system functions:
*************************************************/

void _sys_addTick();
long int _sys_getTicks();

/************************************************
** External namespaces:
*************************************************/
extern struct PagingNamespace Paging;

#endif
