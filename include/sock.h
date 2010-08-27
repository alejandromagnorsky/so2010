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

#include "ipc.h"

struct st_sclient_t {
    int fd;
    char active;
};

ipcdata_t sockIPCData();
/* Generates an ipcdata_t for a INET Domain Socket-based IPC interface */

ipc_t sockConnect(ipcdata_t);
/* Establishes a connection and creates/returns an ipc_t structure ptr */

ipc_t sockServe(ipcdata_t, int nclients);
/* Fires a serving thread and returns an ipc_t structure ptr */

void* sockServeLoop(void* ipc);
/* Starts a serving loop, and loops until ipcdata->sdata.stop is 1.
   Returns errno if there's an error, 0 otherwise. */

int sockDisconnect(ipc_t);
/* Disconnects and destroyes the given ipc_t structure */

#endif
