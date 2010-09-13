/*
**  SISTEMAS OPERATIVOS, 2C 2010
**  PRIMER TRABAJO PRACTICO
**  Simulacion de una colonia de hormigas.

**  File:       sockqueue.h
**  Content:    functions for the socket IPC implementation.
*/
#include "../include/ipc_sock.h"

ipcdata_t sockIPCData(ADDR_TYPE *addr) {
    
    ipcdata_t ret = (ipcdata_t) calloc(1, sizeof(union un_ipcdata_t));
    
    if (ret != NULL) {
        /* Everything else is zeroed, thanks to calloc */
        if (addr != NULL)
            memcpy(&(ret->sdata.addr), addr, ADDR_SIZE);
        else
            memset(&(ret->sdata.addr), 0, ADDR_SIZE);
            
        ret->sdata.status.rstate = SSTATE_ZERO;
        ret->sdata.status.wstate = SSTATE_ZERO;
            
    }
    
    return ret;
    
}

ipc_t sockIPC(ipcdata_t ipcdata) {
    ipc_t ret;
    sstatus_t retstat;
    
    ret = (ipc_t) calloc (1, sizeof(struct st_ipc_t));
    
    ret->status = IPCSTAT_DISCONNECTED;
    ret->maxclts = 0;
    ret->inbox = qnew();
    ret->outbox = qnew();
    
    /* [TODO] deep-copying ipcdata sounds better */
    ret->ipcdata = (ipcdata != NULL) ? ipcdata : sockIPCData(NULL);
    
    return ret;
}

ipc_t sockServe(ipcdata_t ipcdata, int nclients) {

    ipc_t ret;
    int rcreat;
    
    ret = sockIPC(ipcdata);
    ret->maxclts = nclients;
    ret->status = IPCSTAT_PREPARING;
    ret->id = 1;
    rcreat = pthread_create(&(ret->thread), NULL, sockServerLoop, ret);
    
    if (rcreat != 0)
        ret->status = IPCERR_THREAD;

   return ret;

}

ipc_t sockConnect(ipcdata_t ipcdata, int id) {
    
    int rcreat;
    ipc_t ret = (ipc_t) malloc (sizeof(struct st_ipc_t));
    
    ret->status = IPCSTAT_CONNECTING;
    ret->ipcdata = ipcdata; /* [TODO] deep-copying ipcdata sounds better */
    ret->inbox = qnew();
    ret->outbox = qnew();
    ret->id = id;
    
    rcreat = pthread_create(&(ret->thread), NULL, sockClientLoop, ret);
    
    if (rcreat != 0)
        ret->status = IPCERR_THREAD;

   return ret;
    
}

void* sockServerLoop(void* ipcarg) {

    /* [TODO] THIS FUNCTION IS RIDICULOUSLY LONG!! */
    
    ipc_t ipc;              /* The IPC struct ptr: (ipc_t) ipcarg */
    ipcdata_t ipcdata;      /* a pointer to ipc->ipcdata */
    ipc_t client, *clients; /* An array to hold client IPC data + temp ptr */

    /* Temporary and iteration variables: */
    ADDR_TYPE tempaddr;
    int i, fflags, found, tempfd, addrlen;    
    message_t tempm;

    ipc = (ipc_t) ipcarg;
    ipcdata = ipc->ipcdata;
    
    /* Done with the declarations. We need a new, bound, listening socket: */
    
    if ((ipcdata->sdata.fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        ipc->errn = errno;
        ipc->status = IPCERR_SSOCKET;
        return ipcarg;
    }

    if (bind(ipcdata->sdata.fd,
             (struct sockaddr*) &(ipcdata->sdata.addr),
             ADDR_SIZE) == -1) {
        
        ipc->errn = errno;
        ipc->status = IPCERR_SBIND;
        close(ipcdata->sdata.fd);
        return ipcarg;
    }
    
    if (listen(ipcdata->sdata.fd, 5) == -1) {
        ipc->errn = errno;
        ipc->status = IPCERR_SLISTEN;
        close(ipcdata->sdata.fd);
        return ipcarg;
    }

    if ((fflags = fcntl(ipcdata->sdata.fd, F_GETFL, 0)) < 0) {
        ipc->errn = errno;
        ipc->status = IPCERR_SFCNTL;
        close(ipcdata->sdata.fd);
        return ipcarg;
    }
    
    if ((fcntl(ipcdata->sdata.fd, F_SETFL, fflags | O_NONBLOCK)) < 0) {
        ipc->errn = errno;
        ipc->status = IPCERR_SFCNTL;
        close(ipcdata->sdata.fd);
        return ipcarg;
    }
    
    /* Okay, server socket ready for accepting clients! */
    /* We need to set the status to SERVING & allocate space for the clients */
       
    ipc->status = IPCSTAT_SERVING;
    clients = (ipc_t*) malloc(ipc->maxclts * sizeof(ipc_t));
    
    for (i = 0; i < ipc->maxclts; i++)
        clients[i] = sockIPC(NULL);


    while (!(ipc->stop)) {
        
        /* Clients start with IPCSTAT_DISCONNECTED status. A thread is fired to
           handle each client, and when it's done, it sets the status back to
           IPCSTAT_DISCONNECTED so that the slot can be reused */
           
        addrlen = ADDR_SIZE;
        if ((tempfd = accept(ipcdata->sdata.fd,
                             (struct sockaddr *) &tempaddr,
                             &addrlen)) > 0) {
        
            /* New client connected! We need to find him a slot. */        
            for (found = -1, i = 0; found == -1 && i < ipc->maxclts; i++)
                if (clients[i]->status == IPCSTAT_DISCONNECTED)
                    found = i; /* Here! */            
                    
            if (found < 0) {
                /* No room for the new client. Sorry! */
                close(tempfd);
            
            } else {
                /* We found a slot for the new client. */
                client = clients[found];
                client->ipcdata = sockIPCData(&tempaddr);
                client->ipcdata->sdata.fd = tempfd;
                client->status = IPCSTAT_CONNECTING;
                
                pthread_create(&(client->thread), NULL, sockIPCLoop, client);
            }
            
        } else {
            /* Accept call failed. Let's handle active clients: */
            for (i = 0; i < ipc->maxclts; i++) {
            
                while ((tempm = qget(clients[i]->inbox)) != NULL) {
                    qput(ipc->inbox, tempm);
                    mdel(tempm);
                }   
            }
            
            while ((tempm = qget(ipc->outbox)) != NULL) {
                qput(clients[mto(tempm) - 2]->outbox, tempm);
                mdel(tempm);
            }
        }
        
        
   
    } /* End of the loop (when ipc->stop flag is raised) */
    
    for (i = 0; i < ipc->maxclts; i++)
        clients[i]->stop = 1;
    
    close(ipcdata->sdata.fd);

    return ipcarg;
}


void* sockClientLoop(void* ipcarg) {
    
    ipc_t ipc;              /* The IPC struct ptr: (ipc_t) ipcarg */
    ipcdata_t ipcdata;      /* a pointer to ipc->ipcdata */
    
    ipc = (ipc_t) ipcarg;
    ipcdata = ipc->ipcdata;    
    
    if ((ipcdata->sdata.fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        ipc->errn = errno;
        ipc->status = IPCERR_SSOCKET;
        return;
    }

    if ((connect(ipcdata->sdata.fd,
                 (struct sockaddr*) &(ipcdata->sdata.addr),
                 ADDR_SIZE)) == -1) {
                 
        ipc->errn = errno;
        ipc->status = IPCERR_SCONNECT;
        return;
    }
    
    
    return sockIPCLoop(ipcarg);
}

void* sockIPCLoop(void* ipcarg) {
    int fflags;
    ipc_t ipc;              /* The IPC struct ptr: (ipc_t) ipcarg */
    ipcdata_t ipcdata;      /* a pointer to ipc->ipcdata */
    
    message_t temp;
    
    ipc = (ipc_t) ipcarg;
    ipcdata = ipc->ipcdata;
    
    if ((fflags = fcntl(ipcdata->sdata.fd, F_GETFL, 0)) < 0) {
        ipc->errn = errno;
        ipc->status = IPCERR_SFCNTL;
        close(ipcdata->sdata.fd);
        ipc->stop = 1;
    }
    
    if ((fcntl(ipcdata->sdata.fd, F_SETFL, fflags | O_NONBLOCK)) < 0) {
        ipc->errn = errno;
        ipc->status = IPCERR_SFCNTL;
        close(ipcdata->sdata.fd);
        ipc->stop = 1;
    }
    
    ipc->status = IPCSTAT_CONNECTED;
    
    SLOG("(%d) Loop start.\n", getpid()); fflush(stdout);
    
    while (!ipc->stop) {

        sockIPCRecv(ipc);
        sockIPCSend(ipc);
    }

    SLOG("(%d) Loop stop.\n", getpid()); fflush(stdout);

    sockDisconnect(ipc);
    
    return ipcarg;

}

int sockIPCRecv(ipc_t ipc) {
    static int count = 0;
    int offset, rret;
    ipcdata_t ipcdata;
    sstatus_t status;

    ipcdata = ipc->ipcdata;
    status = &(ipc->ipcdata->sdata.status);

    /* We'll read as much as we can, then write as much as we can, and return */

    if (status->rstate == SSTATE_ZERO) {
        /* We are starting the cycle, let's set us as waiting for a header */
        status->rleft = M_HEADER_SIZE;
        status->rstate = SSTATE_HEADER;
        SLOG("(%d) RZero->RHeader\n", getpid()); fflush(stdout);
    }
    
    if (status->rstate == SSTATE_HEADER) {
        /* We are waiting for a full header to be received */
        offset = M_HEADER_SIZE - status->rleft;
;
        if ((rret = read(ipcdata->sdata.fd,
                         &(status->hbuffer) + offset, status->rleft)) != -1) {
            status->rleft -= rret;
            SLOG("(%d) read %d (-%d)\n", getpid(), rret, status->rleft);
        }
        
        fflush(stdout);
        
        if (status->rleft == 0) {
            /* We are finished with the header! */            
            status->inm = mhnew(&(status->hbuffer), NULL); /* See message.h */
            SLOG("(%d) RHeader->RData\n", getpid());
            status->rleft = status->inm->header.len;
            status->rstate = SSTATE_DATA;

        }
    }
    
    if (status->rstate == SSTATE_DATA) {
        /* We are reading the actual message data */
        offset = status->hbuffer.len - status->rleft;

        if ((rret = read(ipcdata->sdata.fd,
                         mdata(status->inm) + offset, status->rleft)) != -1) {
            status->rleft -= rret;
            SLOG("(%d) read %d (-%d)\n", getpid(), rret, status->rleft);
        }
        
        fflush(stdout);
                
        if (status->rleft == 0) {
            /* Finished receiving! */
            SLOG("(%d) Recv: ", getpid());
            //mprintln(status->inm);
            qput(ipc->inbox, status->inm);
            mdel(status->inm);
            status->rstate = SSTATE_ZERO;
        }
    }
    
    return status->rstate;

}

int sockIPCSend(ipc_t ipc) {
    static int count = 0;
    int offset, wret;
    ipcdata_t ipcdata;
    sstatus_t status;

    ipcdata = ipc->ipcdata;
    status = &(ipc->ipcdata->sdata.status);

    /* We'll read as much as we can, then write as much as we can, and return */

    if (status->wstate == SSTATE_ZERO) {
        /* We are starting the cycle, let's see if there's anything to send */
        if ((status->outm = qget(ipc->outbox)) != NULL) {
            SLOG("(%d) WZero->WHeader\n", getpid());fflush(stdout);
            status->wleft = M_HEADER_SIZE;
            status->wstate = SSTATE_HEADER;
        }
    }
    
    if (status->wstate == SSTATE_HEADER) {
        /* We are waiting for a full header to be received */

        offset = M_HEADER_SIZE - status->wleft;

        if ((wret = write(ipcdata->sdata.fd,
                          &(status->outm->header) + offset, status->wleft)) != -1) {
            status->wleft -= wret;
            SLOG("(%d) wrote %d (-%d)\n", getpid(), wret, status->wleft);
        } else
            SLOG("(%d) failed to write (-%d)\n", getpid(), status->wleft);
        
        fflush(stdout);

        if (status->wleft == 0) {
            /* We are finished with the header! */
            SLOG("(%d) WHeader->WData\n", getpid());fflush(stdout);
            status->wleft = status->outm->header.len;
            status->wstate = SSTATE_DATA;

        }
    }

    if (status->wstate == SSTATE_DATA) {
        /* We are reading the actual message data */
        offset = status->outm->header.len - status->wleft;

        if ((wret = write(ipcdata->sdata.fd,
                          mdata(status->outm) + offset, status->wleft)) != -1) {
            status->wleft -= wret;
            SLOG("(%d) wrote %d (-%d)\n", getpid(), wret, status->wleft);
        } else
            SLOG("(%d) failed to write (-%d)\n", getpid(), status->wleft);
        
        fflush(stdout);
        
        if (status->wleft == 0) {
            /* Finished receiving! */
            SLOG("(%d) Sent: ", getpid());
            //mprintln(status->outm);
            mdel(status->outm);
                        SLOG("(%d) WData->WZero\n", getpid());fflush(stdout);
            status->wstate = SSTATE_ZERO;
        }
    }
    
    return status->wstate;
    
}

int sockDisconnect(ipc_t ipc) {
    ipc->stop = 1;
    ipc->errn = 0;
    ipc->maxclts = 0;
    ipc->status = IPCSTAT_DISCONNECTED;
    
    if (ipc->ipcdata != NULL)
        memset(ipc->ipcdata, 0, sizeof(union un_ipcdata_t));
    
    return 1;
};

