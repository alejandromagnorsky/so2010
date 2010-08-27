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
    
    ipc_t ipc;                          /* The IPC struct ptr: (ipc_t) ipcarg */
    int sfd;                            /* The server socket file descriptor */
    fd_set checkfds, readfds, errfds;   /* FD sets for Select */
    
    printf("init");
    /* We'll need an array of clients, a client count, and iteration vars: */
    int i, found, ccount;    
    struct st_sclient_t* clts; /* Future array to store client information */
    struct st_sclient_t* client; /* Will point to a slot. For clearer syntax */
    
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
    /* We need to set the status to SERVING, allocate space for the clients,
       and generate the fdsets for Select. */
       
    ipc->status = IPCSTAT_SERVING;
    
    clts = (struct st_sclient_t*) calloc (ipc->maxclts,
                                         sizeof(struct st_sclient_t));    

    FD_ZERO(&checkfds);
    FD_SET(sfd, &checkfds);
    printf("start");    
    while (!(ipc->stop) && (readfds = errfds = checkfds,
           select(FD_SETSIZE, &readfds, NULL, &errfds, NULL))) {
           
        printf("loop1");
        
        if (FD_ISSET(sfd, &readfds)) {
            /* New client connected! We need to find him a slot. */
            for (found = 0, i = 0; i < ipc->maxclts; i++)
                if (!clts[i].active)
                    found = i;
                    
            if (found != 0) {
                /* We found a slot for the new client. */
                client = &(clts[i]);
                
                if (client->fd = accept(sfd,
                                        (struct sockaddr *) &(client->addr),
                                        &(client->addr_len)) > 0) {
                    /* Success! */
                    client->active = 1;
                    FD_SET(client->fd, &checkfds);
                    
                } else {/* [TODO] Accept call failed. I should do something. */}
            
            } else { /* No slots left. Ignore? [TODO] handle exceptions! */ }
                    
        }
        
        /* We're done checking for new connections. Let's read, and write. */
        printf("loop2");
    }
    
    for (i = 0; i < ipc->maxclts; i++)
        if (clts[i].active) close(clts[i].fd);
    
    close(sfd);
    printf("end");
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

