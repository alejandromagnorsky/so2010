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

ipcdata_t sockIPCData() {
    
    ipcdata_t ret = (ipcdata_t) malloc(sizeof(union un_ipcdata_t));
    
    if (ret != NULL) {
        ret->sdata.fd = 0;
        memcpy(&(ret->sdata.addr), &DEFAULT_ADDR, ADDR_SIZE);
    }
    
    return ret;
    
}

ipc_t sockServe(ipcdata_t ipcdata, int nclients) {

    int rcreat;
    ipc_t ret = (ipc_t) calloc (1, sizeof(struct st_ipc_t));
    
    ret->status = IPCSTAT_PREPARING;
    ret->maxclts = nclients;
    ret->ipcdata = ipcdata; /* [TODO] deep-copying ipcdata sounds better */
    ret->inbox = qnew();
    ret->outbox = qnew();

    rcreat = pthread_create(&(ret->thread), NULL, sockServerLoop, ret);
    
    if (rcreat != 0)
        ret->status = IPCERR_THREAD;

   return ret;

}

void* sockServerLoop(void* ipcarg) {
    
    ipc_t ipc;
    int i, sfd, ccount;
    struct st_sclient_t* clts;
    
    ipc = (ipc_t) ipcarg;
    
    if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        ipc->errn = errno;
        ipc->status = IPCERR_SSOCKET;
        return;
    }

    if (bind(sfd, (struct sockaddr*) &(ipc->ipcdata->sdata.addr), ADDR_SIZE) == -1) {
        ipc->errn = errno;
        ipc->status = IPCERR_SBIND;
        close(sfd);
        return;
    }
    
    if (listen(sfd, 5) == -1) {
        ipc->errn = errno;
        ipc->status = IPCERR_SLISTEN;
        close(sfd);
        return;
    }
    
    /* Okay, server socket ready for accepting clients! */
    ipc->status = IPCSTAT_SERVING;
    
    clts = (struct st_sclient_t*) calloc (ipc->maxclts,
                                         sizeof(struct st_sclient_t));
    
    while (!(ipc->stop)) {
        
        /* trabajo */
    
    }
    
    for (i = 0; i < ipc->maxclts; i++)
        if (clts[i].active) close(clts[i].fd);
    
    close(sfd);
    
    return;
}


ipc_t sockConnect(ipcdata_t ipcdata) {
    
    int rcreat;
    ipc_t ret = (ipc_t) malloc (sizeof(struct st_ipc_t));
    
    ret->status = IPCSTAT_CONNECTING;
    ret->ipcdata = ipcdata; /* [TODO] deep-copying ipcdata sounds better */
    ret->inbox = qnew();
    ret->outbox = qnew();
    
    rcreat = pthread_create(&(ret->thread), NULL, sockClientLoop, ret);
    
    if (rcreat != 0)
        ret->status = IPCERR_THREAD;

   return ret;
    
}

void* sockClientLoop(void* ipcarg) {
    
    ipc_t ipc;
    int cfd;
    
    ipc = (ipc_t) ipcarg;
    
    if ((cfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        ipc->errn = errno;
        ipc->status = IPCERR_SSOCKET;
        return;
    }
    
    if ((connect(cfd, (struct sockaddr*) &(ipc->ipcdata->sdata.addr), ADDR_SIZE)) == -1) {
        ipc->errn = errno;
        ipc->status = IPCERR_SCONNECT;
        return;
    }
    
    /* Ready to rumble! */
    ipc->status = IPCSTAT_CONNECTED;
    
    while (!ipc->stop) {
    
        /* send/recv loop */
    
    }
    
    close(cfd);
    return;
}

int sockDisconnect(ipc_t);
/* Disconnects and destroyes the given ipc_t structure */

