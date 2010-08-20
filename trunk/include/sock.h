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
#include <sys/types.h>
#include <sys/socket.h>   

#include "ipc.h"

ipcdata_t sockIPCData(char* path);
/* Generates an ipcdata_t for a Unix Domain Socket-based IPC interface */

ipc_t sockConnect(ipcdata_t);
/* Establishes a connection and creates/returns an ipc_t structure */

int sockDisconnect(ipc_t);
/* Disconnects and destroyes the given ipc_t structure */

#endif
