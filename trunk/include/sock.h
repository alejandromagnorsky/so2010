/*
**  SISTEMAS OPERATIVOS, 2C 2010
**  PRIMER TRABAJO PRACTICO
**  Simulacion de una colonia de hormigas.

**  File:       sockqueue.h
**  Content:    header file for the socket IPC implementation.
*/
   
#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>  
#include <sys/time.h> 

#include "ipc.h"

enum {
    SCLIENT_STATE_ZERO,
    SCLIENT_STATE_HEADER,
    SCLIENT_STATE_DATA,
    SCLIENT_STATE_READY
};

struct st_sclient_t {
    /* This structure will hold client information for the socket server. */
    int fd;
    char active;
    int rstate, wstate;
        
    int addr_len;
    struct sockaddr_in addr;
    
    /* Below, buffers and byte counters for reading and writing the socket */
    int rleft, wleft;
    struct st_mheader_t header;
    
    message_t inm;
    message_t outm;
};

typedef struct st_sclient_t* sclient_t;

ipcdata_t sockIPCData();
/* Generates an ipcdata_t for a INET Domain Socket-based IPC interface */

ipc_t sockConnect(ipcdata_t);
/* Fires a thread to connect to a server, returns a ipc_t structure ptr. */

void* sockClientLoop(void* ipc);
/* Starts a connection loop. Loops until ipc->stop is 1.
   To be called in a new thread */

ipc_t sockServe(ipcdata_t, int nclients);
/* Fires a serving thread and returns an ipc_t structure ptr. */

sclient_t* sockClientsAlloc(int n);
/* Allocates necessary memory for n client structres, and initializes them. */

void* sockServerLoop(void* ipc);
/* Starts a serving loop, and loops until ipc->stop is 1.
   To be called in a new thread. */ 

char handleClientRecv(sclient_t);
/* Executes the read cycle of the client. Returns true if this call completed
   the cycle (a full message with header and data arrived, in client->inm) */

char handleClientSend(sclient_t);

int sockDisconnect(ipc_t);
/* Disconnects and destroys the given ipc_t structure. */

#endif
