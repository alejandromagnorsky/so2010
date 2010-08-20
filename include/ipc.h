/*
**  SISTEMAS OPERATIVOS, 2C 2010
**  PRIMER TRABAJO PRACTICO
**  Simulacion de una colonia de hormigas.

**  File:       ipc.h
**  Content:    header file for the IPC abstract interface.
*/

#ifndef __IPC_H__
#define __IPC_H__

#include "queue.h"
#include "message.h"

#include <sys/types.h>
#include <sys/socket.h>

enum {
    IPCSTAT_DISCONNECTED
    IPCSTAT_CONNECTING
    IPCSTAT_CONNECTED
    IPCSTAT_ERROR
}

union un_ipcdata_t {
    
    struct {
        int fd;
        struct sockaddr_un sunaddr;
    } sundata;
    
    struct {
        int fdr;
        int fdw;
        fifoname[100];
    } fifodata;
    
    /* Message queues? */
    
};

typedef st_ipcdata_t* ipcdata_t;

struct st_ipc_t {
    int status;
    ipcdata_t ipcdata;
    queue_t inbox, outbox;
}

typedef struct st_ipc_t* ipc_t;

int sendMessage(ipc_t, message_t);
/* Queues a message for sending. */

message_t recvMessage(ipc_t);
/* Returns messages in order of arrival, or NULL if there are none */

#endif
