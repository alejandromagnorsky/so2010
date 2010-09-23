#ifndef _ipc_queue_
#define _ipc_queue_

#include "../include/ipc.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define QKEY		(key_t)0105
#define QPERM		0660
#define MAXOBN		200
#define MAXPRIOR	100000

#define SERVERKEY 1

#define CLIENTSENDPRIOR 3
#define CLIENTRECVPRIOR 4

#define CLIENT2SENDPRIOR 5
#define CLIENT2RECVPRIOR 6


struct q_entry {
	long mtype;
	char mtext[MAXOBN];
};

ipcdata_t mq_ipcdata(int sendprior, int recvprior);
/* Returns the data needed for the ipc */

ipc_t mq_connect(ipcdata_t ipcdata);
/* Fires a thread to connect to a client, returns a ipc_t structure ptr. */

ipc_t mq_serve(ipcdata_t ipcdata);
/* Fires a thread to connect to a server, returns a ipc_t structure ptr. */

void * mq_serverLoop(void* ipcarg);
/* Starts a connection loop. Loops until ipc->stop is 1.
   To be called in a new thread */
   
void * mq_clientLoop(void* ipcarg);

int init_queue();
/* Opens or connects to a queue as required. */

int mq_sendData(ipc_t ipc, message_t msg, int priority);
/* Sends the given message to the queue given by the ipc. 
	If it is not connected it opens or creates a queue*/

message_t mq_getData(ipc_t ipc, int priority);
/* Receives a message from the queue according to the given priority.
	If it is not connected it opens or creates a queue*/

void mq_disconnect(ipc_t ipc);
/* Disconnects and destroys the given ipc_t structure. */

void warn(char * s);
/* It prints an error in stderr. */

#endif
