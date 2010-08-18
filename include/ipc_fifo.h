#ifndef IPC_FIFO_H_
#define IPC_FIFO_H_

#include "../include/ipc.h"

#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#define	KEYSIZE	4

#define PERMISSIONS	0666

int keyToInt(char * key);
int intToKey(char * keyBuf, int len);

#endif
