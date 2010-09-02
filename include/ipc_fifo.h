#ifndef IPC_FIFO_H_
#define IPC_FIFO_H_

#include "ipc.h"

#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>

#define PERMISSIONS	0666

int initFifos(int nClients);
/*Initializes the necessary amount of FIFO's */

ipcdata_t fifoIPCData(int nant);
/* Generates an ipcdata_t with the corresponding fifo's name to write and read */

ipc_t fifoConnect(ipcdata_t ipcdata);
/* Fires a thread to connect to a server */

int writefifo(ipc_t ipc, void * data, int len);
/* Write in the corresponding FIFO of the ipc */

int readfifo(ipc_t ipc, char * buffer, int len);
/* Read the corresponding FIFO of the ipc*/

int fifoDisconnect(ipc_t ipc);
/* Disconnects and destroys the ipc*/

void* fifoClientLoop(void* ipcarg);
/* Receives & Send messages in the inbox and outbox */

void* fifoServerLoop(void * ipcarg);
/* */

/* Structures to Receive & Send messages */
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


struct st_client_t {
	ipcdata_t cinfo;
	msg_reading msgr;
	msg_writting msgw;
};

typedef struct st_client_t* client_t;

#endif
