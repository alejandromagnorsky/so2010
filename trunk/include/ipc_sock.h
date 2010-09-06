/*
**  SISTEMAS OPERATIVOS, 2C 2010
**  PRIMER TRABAJO PRACTICO
**  Simulacion de una colonia de hormigas.

**  File:       sockqueue.h
**  Content:    header file for the socket IPC implementation.
*/
   
#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h> 
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>

#include "ipc.h"

#define SLOG(...) {}

enum {
    SSTATE_ZERO,
    SSTATE_HEADER,
    SSTATE_DATA,
};

#define ADDR_TYPE struct sockaddr_in
#define ADDR_SIZE (sizeof(ADDR_TYPE))

ipcdata_t sockIPCData(ADDR_TYPE*);
/* Generates an ipcdata_t for an INET Domain Socket-based IPC interface */

ipc_t sockIPC(ipcdata_t ipcdata);
/* Generates an IPC structure from given ipcdata, without connecting anything */

ipc_t sockServe(ipcdata_t ipcdata, int nclients);
/* Creates a serving thread and returns the appropiate ipc_t structre */

ipc_t sockConnect(ipcdata_t);
/* Establishes a connection and creates/returns an ipc_t structure */

void* sockServerLoop(void* ipcarg);
/* Function to actually serve clients, passed to pthread_create */

void* sockClientLoop(void* ipcarg);
/* Function to actually connect to a server, passed to pthread_create */

void* sockIPCLoop(void* ipcarg);
/* Function to loop over read/write calls and empty/populate queues */

int sockIPCRecv(ipc_t ipc);
/* One iteration of the message recv state machine */

int sockIPCSend(ipc_t ipc);
/* One iteration of the message send state machine */

int sockRead(ipc_t ipc, char* buffer, int nbytes);
/* LOW-LEVEL FUNCTION to read data, DO NOT USE outside this module */

int sockWrite(ipc_t ipc, char* buffer, int nbytes);
/* LOW-LEVEL FUNCTION to write data, DO NOT USE outside this module */

int sockDisconnect(ipc_t);
/* Disconnects and destroyes the given ipc_t structure */

#endif
