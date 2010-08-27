#ifndef _ipc_queue_
#define _ipc_queue_

#include "../include/ipc.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#define QKEY		(key_t)0105
#define QPERM		0660
#define MAXOBN		50
#define MAXPRIOR	100


struct q_entry {
	long mtype;
	char mtext[MAXOBN+1];
};

ipc_t mq_connect();
void * mq_serverLoop(void* ipcarg);
int init_queue(void);
int mq_sendData(ipc_t ipc, message_t msg);
message_t mq_getData(ipc_t ipc, int priority);
void mq_disconnect(ipc_t ipc);
int warn(char * s);

#endif
