#ifndef IPC_H_
#define IPC_H_

#include "../include/message.h"

struct st_ipc_t {
	int status;
	char * path; // IF FIFO.
};

typedef struct st_ipc_t * ipc_t;

ipc_t connect(char * path);

int sendMessage(ipc_t ipc, message_t msg);

message_t getMessage();

int disconnect(ipc_t ipc);

#endif
