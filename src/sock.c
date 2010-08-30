/*
**  SISTEMAS OPERATIVOS, 2C 2010
**  PRIMER TRABAJO PRACTICO
**  Simulacion de una colonia de hormigas.

**  File:       sockqueue.h
**  Content:    functions for the socket IPC implementation.
*/

#include "../include/sock.h"

#define ADDR_SIZE (sizeof(struct sockaddr_in))
struct sockaddr_in DEFAULT_ADDR = {AF_INET, 4547, INADDR_ANY};

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

    /* [TODO] THIS FUNCTION IS RIDICULOUSLY LONG!! */
    
    ipc_t ipc;                          /* The IPC struct ptr: (ipc_t) ipcarg */
    int sfd;                            /* The server socket file descriptor */
    fd_set checkfds, readfds, errfds, writefds; /* FD sets for Select */
    
    /* We'll need an array of clients, a client count, and iteration vars: */
    int i, found, ccount, left, offset;    
    
    sclient_t clts;     /* Future array to store client information */
    sclient_t client;   /* Will point to a slot. For clearer syntax */
    
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
    ccount = 0;
    clts = (sclient_t) calloc(ipc->maxclts, sizeof(struct st_sclient_t));

    FD_ZERO(&checkfds);
    FD_SET(sfd, &checkfds);

    while (!(ipc->stop) && (readfds = errfds = writefds = checkfds,
           select(FD_SETSIZE, &readfds, &writefds, &errfds, NULL))) {
        
        if (FD_ISSET(sfd, &readfds)) {
            /* New client connected! We need to find him a slot. */
            
            for (found = -1, i = 0; found == -1 && i < ipc->maxclts; i++)
                if (clts[i].active == 0)
                    found = i;
                    
            if (found > -1) {
                /* We found a slot for the new client. */
                client = &(clts[found]);
                
                if ((client->fd = accept(sfd,
                                        (struct sockaddr *) &(client->addr),
                                        &(client->addr_len))) > 0) {
                    /* Success! */
                    ccount++;
                    client->active = 1;
                    FD_SET(client->fd, &checkfds);
                    
                    fflush(stdout);
                    
                } else {
                    printf("accept failed!\n");
                    fflush(stdout);
                    /* [TODO] Accept call failed. I should do something. */
                }
            
            } else { /* No slots left. Ignore? [TODO] handle exceptions! */ }
                    
        }
        
        /* We're done checking for new connections. Let's read, and write. */
        for (i = 0; i < ipc->maxclts; client = &(clts[i++])) {
            
            /* We're going to read, then write, then check for errors */
            if (client->active && FD_ISSET(client->fd, &readfds)) {
                /* Data available! */

                 if (handleClientRecv(client)) {
                    mprintln(client->inm);
                    mdel(client->inm);
                    
                    client->inm = NULL;
                    client->rstate = SCLIENT_STATE_ZERO;
                 }
            
            } /* End if (data is available for reading) */
            
        } /* End for (check all clients) */
        
    } /* End select loop (when ipc->stop is raised) */
    
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
    int left, offset;
    struct st_sclient_t clientstr = {};
    sclient_t client = &clientstr;
    
    ipc = (ipc_t) ipcarg;
    
    if ((client->fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        ipc->errn = errno;
        ipc->status = IPCERR_SSOCKET;
        return;
    }
    
    if ((connect(client->fd, (struct sockaddr*) &(ipc->ipcdata->sdata.addr), ADDR_SIZE)) == -1) {
        ipc->errn = errno;
        ipc->status = IPCERR_SCONNECT;
        return;
    }
    
    /* Ready to rumble! */
    ipc->status = IPCSTAT_CONNECTED;
    
    while (!ipc->stop) {
    
        if (client->outm == NULL)
           client->outm = qget(ipc->outbox);

        if (handleClientSend(client)) {
            mdel(client->outm);
            client->outm = NULL;
            client->wstate = SCLIENT_STATE_ZERO;
        }
        
    }
    
    close(client->fd);
    return;
}

char handleClientRecv(sclient_t client) {

    int offset;

    /* We'll read as much as we can, then write as much as we can, and return */

    if (client->rstate == SCLIENT_STATE_ZERO) {
        /* We are starting the cycle, let's set us as waiting for a header */
        client->rleft = M_HEADER_SIZE;
        client->rstate = SCLIENT_STATE_HEADER;
    }
    
    if (client->rstate == SCLIENT_STATE_HEADER) {
        /* We are waiting for a full header to be received */
        offset = M_HEADER_SIZE - client->rleft;
        
        client->rleft -= read(client->fd, &(client->header) + offset, client->rleft);
        
        if (client->rleft == 0) {
            /* We are finished with the header! */            
            client->inm = mhnew(&(client->header), NULL); /* See message.h */

            client->rleft = client->inm->header.len;
            client->rstate = SCLIENT_STATE_DATA;

        }
    }
    
    if (client->rstate == SCLIENT_STATE_DATA) {
        /* We are reading the actual message data */
        offset = client->header.len - client->rleft;
        
        client->rleft -= (client->fd, mdata(client->inm) + offset, client->rleft);
        
        if (client->rleft == 0) {
            /* Finished receiving! */
            client->rstate = SCLIENT_STATE_READY;
        }
    }
    
    return (client->rstate == SCLIENT_STATE_READY);

}

char handleClientSend(sclient_t client) {

    int offset;

    if (client->wstate == SCLIENT_STATE_ZERO) {
    
        if (client->outm != NULL) {
            /* We are starting the cycle, let's set us as waiting for a header */
            memcpy(&(client->header), &(client->outm->header), M_HEADER_SIZE);
            client->wleft = M_HEADER_SIZE;
            client->wstate = SCLIENT_STATE_HEADER;
        }
        
    }

    if (client->wstate == SCLIENT_STATE_HEADER) {
        /* We are waiting for to finish sending the full header */
        offset = M_HEADER_SIZE - client->wleft;

        printf("head[%*s]", client->wleft, &client->header + offset);
        client->wleft -= write(client->fd, &(client->header) + offset, client->wleft);
        
        if (client->wleft == 0) {
            /* We are finished with the header! */
            client->wleft = client->outm->header.len;
            client->wstate = SCLIENT_STATE_DATA;
        }
    }
    
    if (client->wstate == SCLIENT_STATE_DATA) {
        /* We are sending the actual message data */
        offset = client->header.len - client->wleft;

        printf("data[%*s]",client->wleft, mdata(client->outm) + offset);
        printf("len: %d, offset:%d left:%d, %*s", client->header.len, offset, client->wleft, client->wleft, mdata(client->outm));
        client->wleft -= write(client->fd, mdata(client->outm) + offset, client->wleft);
        
        if (client->wleft == 0) {
            /* Finished sending! */
            client->wstate = SCLIENT_STATE_READY;
        }
    }
    
    return (client->wstate == SCLIENT_STATE_READY);

}

int sockDisconnect(ipc_t);

