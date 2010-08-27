/*
**  SISTEMAS OPERATIVOS, 2C 2010
**  PRIMER TRABAJO PRACTICO
**  Simulacion de una colonia de hormigas.

**  File:       sockqueue.h
**  Content:    functions for the socket IPC implementation.
*/

#include "../include/sock.h"

#define ADDR_SIZE (sizeof(struct sockaddr_in))
struct sockaddr_in DEFAULT_ADDR = {AF_INET, 4545, INADDR_ANY};

ipcdata_t sockIPCData(int port) {
    
    ipcdata_t ret = (ipcdata_t) malloc(sizeof(union un_ipcdata_t));
    
    if (ret != NULL) {
        ret->sdata.fd = 0;
        memcpy(&(ret->sdata.addr), &DEFAULT_ADDR, ADDR_SIZE);
    }
    
    return ret;
    
}

ipc_t sockServe(ipcdata_t ipcdata) {
    ipc_t ret = (ipc_t) malloc (sizeof(struct st_ipc_t));
    
    ret->status = IPCSTAT_DISCONNECTED;
    ret->inbox = qnew();
    ret->outbox = qnew();

}

int sockServeLoop(ipc_t ipc, int maxcl) {

    int i, sfd, ccount, ret;
    struct st_client_t* clts;
    
    if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        return errno;
        
    if (bind(sfd, (struct sockaddr*) &(ipcdata->sdata.addr), ADDR_SIZE) == -1) {
        ret = errno;
        close(sfd);
        return ret;
    }
    
    if (listen(sfd, 5) == -1) {
        ret = errno;
        close(sfd);
        return ret;
    }
    
    /* Okay, server socket ready for accepting clients! */
    clts = (struct st_client_t*) calloc (maxcl, sizeof(struct st_client_t));
    
    while (!ipc->stop) {
        
        /* select loop */
    
    }
    
    for (i = 0; i < maxcl; i++)
        if (clts[i].active) close(clts[i].fd);
    
    close(sfd);
    
    return 0;
}


ipc_t sockConnect(ipcdata_t) {
    
    ipc_t ret = (ipc_t) malloc (sizeof(struct st_ipc_t));
    
    ret->status = IPCSTAT_DISCONNECTED;
    
    
    
    ret->inbox = qnew();
    ret->outbox = qnew();
    
    
    
}

int sockDisconnect(ipc_t);
/* Disconnects and destroyes the given ipc_t structure */

