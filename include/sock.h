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

struct st_sclient_t {
    int fd;
    char active;
    message_t buf;
};

ipcdata_t sockIPCData();
/* Generates an ipcdata_t for a INET Domain Socket-based IPC interface */

ipc_t sockConnect(ipcdata_t);
/* Fires a thread to connect to a server, returns a ipc_t structure ptr. */

void* sockClientLoop(void* ipc);
/* Starts a connection loop. Loops until ipc->stop is 1.
   To be called in a new thread */

ipc_t sockServe(ipcdata_t, int nclients);
/* Fires a serving thread and returns an ipc_t structure ptr. */

void* sockServerLoop(void* ipc);
/* Starts a serving loop, and loops until ipc->stop is 1.
   To be called in a new thread. */

int sockDisconnect(ipc_t);
/* Disconnects and destroys the given ipc_t structure. */

#endif
