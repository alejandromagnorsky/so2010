/*
**  SISTEMAS OPERATIVOS, 2C 2010
**  PRIMER TRABAJO PRACTICO
**  Simulacion de una colonia de hormigas.

**  File:       main.c
**  Content:    main executable.
*/

#include "../include/main.h"

#define ANTS 10

int main(int argc, char** argv) {

    int sid, status;
    char forking;
    message_t message;
    pid_t pid, cpid;
    ipc_t ipc;
    
    LOGPID("Using IPC method: %s.\n", IPC_METHOD);
    
    /* Before doing anything else, map data should be loaded! */
    /* (so as to know the number of ants beforehand, at least */

    /* This process will act as IPC server/simulation control */
    
    sid = 1;        /* Control has simulation ID 1 */
    pid = getpid(); /* and it's pid, obviously */

    LOGPID("Control process started.\n");    
    ipc = initServer();

    /* Good! IPC server working. Let's spawn those ants. */
    
    if (cpid = fork()) {
    
        /* Control code here */
        while(1) {
            fflush(stdout);
            if (message = recvMessage(ipc)) {
                LOGPID("Control received: ");
                sendMessage(ipc, mnew(1, mfrom(message),
                                      sizeof(struct cmd_start_t),
                                      (char*) newStart()));
                mprintln(message);
                mdel(message);
            }
        }
    } else {       
    
        /* Ants here */
        do {
            sid++;
            pid = getpid();
                        
            if (forking = (sid - 1 < ANTS))
                forking = ((cpid = fork()) != 0); /* Child will keep forking */
        
        } while (forking);
        
        /* We can do our own stuff now */           
        ipc = initClient();
        ipc->id = sid; /* Grasaaaa! */

        status = antLoop(ipc);
        exit(status);
        
    }
    
}

ipc_t initServer() {
    ipc_t ipc;
    ipcdata_t ipcdata;
    
    LOGPID("Creating IPCData for server...\n"); /* Macro defined in tools.h */
    
    if ((ipcdata = IPCF_IPCDATA(IPCF_IPCDATA_ARGS)) != NULL)
        LOGPID("Created IPCData for server.\n");
    else {
        LOGPID("Failed to create IPCData for server!\n");
        exit(1);
    }
    
    LOGPID("Attempting to serve...\n");
    
    ipc = IPCF_SERVE(ipcdata, 10);
    
    while(ipc->status == IPCSTAT_PREPARING);

    LOGPID("Serving attempt status %d (errno %d)\n", ipc->status, ipc->errn);
    
    if (ipc->status == IPCSTAT_SERVING)
        LOGPID("Serving!\n");
        
    else {
        LOGPID("Serving attempt failed!\n");
        exit(1);
    }
    
    return ipc;
    
}

ipc_t initClient() {
    ipc_t ipc;
    ipcdata_t ipcdata;

    LOGPID("Creating IPCData for client...\n"); /* Macro defined in tools.h */
    
    if ((ipcdata = IPCF_IPCDATA(IPCF_IPCDATA_ARGS)) != NULL)
        LOGPID("Created IPCData for client.\n");
    else {
        LOGPID("Failed to create IPCData for client!\n");
        exit(1);
    }
    
    LOGPID("Attempting to connect...\n");

    ipc = IPCF_CONNECT(ipcdata);

    while (ipc->status == IPCSTAT_CONNECTING);
    
    LOGPID("Connect attempt status %d (errno %d)\n", ipc->status, ipc->errn);

    if (ipc->status == IPCSTAT_CONNECTED)
        LOGPID("Connected ok!\n");
        
    else {
        LOG("Failed to connect!\n");
        exit(1);
    }
    
    return ipc;
}
