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


/* Structures to Receive & Send messages */
struct st_msg_writting {
	int msglen;
	int toWrite;
	char * data;
	int to;
	int from;
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
	msg_reading currMsgR;
};

typedef struct st_client_t* client_t;



int initFifos(int nClients);
/*Initializes the necessary amount of FIFO's */

ipcdata_t fifoIPCData(int nant);
/* Generates an ipcdata_t with the corresponding fifo's name to write and read */

ipc_t fifoConnect(ipcdata_t ipcdata);
/* Fires a thread to connect to a server */

ipc_t fifoServe(int nclients);
/* Fires a server for nclients*/

int fifoDisconnect(ipc_t ipc);
/* Disconnects and destroys the ipc*/

void* fifoClientLoop(void* ipcarg);
/* Receives & Send messages in the inbox and outbox for the client */

void* fifoServerLoop(void * ipcarg);
/* Receives & Send messages in the inbox and outbox for the server */

void stopServer(ipc_t server);
/* Stop the server and free the memory allocated */

void fifoHandlerRead(ipc_t ipc, int fdr, queue_t inbox, msg_reading currMsgR);
void fifoHandlerWrite(ipc_t ipc, int fdw, msg_writting currMsgW);
void fifoHandlerWriteServer(ipc_t ipc, msg_writting currMsgW, client_t * clients);

void initializeClientsServer(ipc_t ipc, client_t * clients);
void finalizeClientsServer(ipc_t ipc, client_t * clients);

#endif
