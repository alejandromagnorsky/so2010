#ifndef IPC_FIFO_H_
#define IPC_FIFO_H_

#include "ipc.h"

#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>

#define	KEYSIZE	4

#define PERMISSIONS	0666

ipcdata_t fifoIPCData(int nant);
ipc_t fifoConnect(ipcdata_t ipcdata);
int writefifo(ipc_t ipc, void * data, int len);
int readfifo(ipc_t ipc, char * buffer, int len);
int fifoDisconnect(ipc_t ipc);

void* fifoClientLoop(void* ipcarg);

struct st_msg_writting {
	int written;
	int toWrite;
	char * data;
};

typedef struct st_msg_writting* msg_writting;

struct st_msg_reading {
	int read;
	int toRead;
	message_t recv;
};

typedef struct st_msg_reading* msg_reading;

#endif
