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

int initFifos(int qtyAnts);
void unlinkFifos(int qtyAnts);

ipcdata_t fifoIPCData(int nant);
ipc_t fifoConnect(ipcdata_t ipcdata);
int writefifo(ipc_t ipc, void * data, int len);
int readfifo(ipc_t ipc, char * buffer, int len);
int fifoDisconnect(ipc_t ipc);

void* fifoClientLoop(void* ipcarg);

struct st_msg_writting {
	int msglen;
	int toWrite;
	char * data;
};

struct st_msg_reading {
	int toRead;
	int hdread;
	char bufferhd[M_HEADER_SIZE];
	message_t incomingMsg;
};

typedef struct st_msg_reading* msg_reading;
typedef struct st_msg_writting* msg_writting;


#endif
