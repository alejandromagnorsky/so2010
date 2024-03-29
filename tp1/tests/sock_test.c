/*
**  SISTEMAS OPERATIVOS, 2C 2010
**  PRIMER TRABAJO PRACTICO
**  Simulacion de una colonia de hormigas.

**  File:       main.c
**  Content:    main executable.
*/

#include "../include/main.h"

int sid;

int main(int argc, char** argv) {

    int status;
    char forking;
    message_t message;
    pid_t pid, cpid;
    ipc_t ipc;
    grid_t grid = gnew();
    
    LOGPID("Using IPC method: %s.\n", IPC_METHOD);
    
    /* Before doing anything else, map data should be loaded! */
    /* (so as to know the number of ants beforehand, at least */
    
	if((status = loadGrid(grid, "configurationFile")) != NO_ERRORS)
	{
		LOGPID("An error occurred while loading the configuration file\n ");
		exit(status);
	}
	
    /* This process will act as IPC server/simulation control */
    
    sid = 1;        /* Control has simulation ID 1 */
    pid = getpid(); /* and it's pid, obviously */

    LOGPID("Control process started.\n");    
    ipc = initServer();

    /* Good! IPC server working. Let's spawn those ants. */
    
    if (cpid = fork()) {
    
        /* Control code here */        
    	int aux;
    	if((aux = launchControl(ipc, grid)) != NO_ERROR){
    		LOGPID("Simulation fail: %d\n", aux );
    	}else{
    		LOGPID("Simulation ended succesfully!\n");
    	}
    } else {       
    
        /* Ants here */
        do {
            sid++;
            pid = getpid();
            ipc = initClient();
            if (forking = (sid - 1 < grid->antsQuant))
                forking = ((cpid = fork()) == 0); /* Child will keep forking */
        } while (forking);
        
        /* We can do our own stuff now */                  
        status = antLoop(ipc);
        exit(status);
        
    }
    
    //[TODO] free grid
}

ipc_t initServer() {
    ipc_t ipc;
    ipcdata_t ipcdata;

#ifndef IPC_METHOD_FIFOS
#ifndef IPC_METHOD_SHMEM
    
    LOGPID("Creating IPCData for server...\n"); /* Macro defined in tools.h */
    if ((ipcdata = IPCF_IPCDATA(IPCF_IPCDATA_ARGS)) != NULL)
        LOGPID("Created IPCData for server.\n");
    else {
        LOGPID("Failed to create IPCData for server!\n");
        exit(1);
    }
#endif
#endif
    LOGPID("Attempting to serve...\n");

    ipc = IPCF_SERVE(ipcdata, 10);
    
#ifndef IPC_METHOD_FIFOS
#ifndef IPC_METHOD_SHMEM    
    while(ipc->status == IPCSTAT_PREPARING);

    LOGPID("Serving attempt status %d (errno %d)\n", ipc->status, ipc->errn);
    
    if (ipc->status == IPCSTAT_SERVING)
        LOGPID("Serving!\n");
        
    else {
        LOGPID("Serving attempt failed!\n");
        exit(1);
    }
#endif
#endif
    
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

    ipc = IPCF_CONNECT(ipcdata, sid);

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
