#ifndef IPC_FIFO_H_
#define IPC_FIFO_H_

#include "ipc.h"

#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#define	KEYSIZE	4

#define PERMISSIONS	0666

ipcdata_t fifoIPCData(int nant);
ipc_t fifoConnect(ipcdata_t ipcdata);
int writefifo(ipc_t ipc, void * data, int len);
int readfifo(ipc_t ipc, char * buffer, int len);
int fifoDisconnect(ipc_t ipc);

void* fifoClientLoop(void* ipcarg);

#endif
