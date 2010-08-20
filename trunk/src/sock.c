/*
**  SISTEMAS OPERATIVOS, 2C 2010
**  PRIMER TRABAJO PRACTICO
**  Simulacion de una colonia de hormigas.

**  File:       sockqueue.h
**  Content:    functions for the socket IPC implementation.
*/

#include "../include/sock.h"

ipcdata_t sockIPCData(char* path) {
    
    ipcdata_t ret = (ipcdata_t) malloc(sizeof(union un_ipcdata_t));
    
    if (ret != NULL) {
        ret->sundata.sunaddr.sun_family = AF_UNIX;
        strncpy(ret->sundata.sunaddr.sun_path, path, 108);
    }
    
    return ret;
    
}

ipc_t sockConnect(ipcdata_t) {
    
    ipc_t ret = (ipc_t) malloc (sizeof(struct st_ipc_t));
    
    ret->status = IPCSTAT_DISCONNECTED;
    ret->inbox = qnew();
    ret->outbox = qnew();
    
}

int sockDisconnect(ipc_t);
/* Disconnects and destroyes the given ipc_t structure */

