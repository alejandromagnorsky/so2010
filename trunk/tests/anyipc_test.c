/*
**  SISTEMAS OPERATIVOS, 2C 2010
**  PRIMER TRABAJO PRACTICO
**  Simulacion de una colonia de hormigas.

**  File:       sock_test.c
**  Content:    tests for the socket IPC library.
*/

#include <stdio.h>
#include <stdlib.h>


#define IPC_METHOD_SOCKETS
//#define IPC_METHOD_MQS
//#define IPC_METHOD_FIFOS
//#define IPC_METHOD_SHMEM

#ifdef IPC_METHOD_SOCKETS
    #include "../include/ipc_sock.h"
    struct sockaddr_in DEFAULT_ADDR = {AF_INET, 4545, INADDR_ANY};

    // Requirements to build IPCData vary from IPC method to IPC method
    #define IPCF_IPCDATA(...)   sockIPCData(__VA_ARGS__)
    #define IPCF_IPCDATA_ARGS   &DEFAULT_ADDR
    
    // IPCF_SERVE always receives ipcdata_t and the max. number of clients
    #define IPCF_SERVE(X, Y)    sockServe((X), (Y))

    // IPCF_CONNECT always receives ipcdata_t
    #define IPCF_CONNECT(X, Y)     sockConnect(X, Y)
#endif

#ifdef IPC_METHOD_MQS
	#include "../include/ipc_queue.h"
	
	// Requirements to build IPCData vary from IPC method to IPC method
    #define IPCF_IPCDATA(...)   mq_ipcdata(__VA_ARGS__)
    #define IPCF_IPCDATA_ARGS   (getpid(), getpid())
    
    // IPCF_SERVE always receives ipcdata_t
    #define IPCF_SERVE(X, Y)    mq_serve(X)

    // IPCF_CONNECT always receives ipcdata_t
    #define IPCF_CONNECT(X)     mq_connect(X)
#endif

int main(int argc, char** argv) {

    pid_t cpid, spid, pid;
    message_t temp;
    ipcdata_t ipcdata, ipcdata2;
    ipc_t sipc, cipc;
    
    char stop = 0;
    LOG("Creating IPCData for server... "); // Macro defined in tools.h
    
    if ((ipcdata = IPCF_IPCDATA(IPCF_IPCDATA_ARGS)) != NULL)
        LOG("ok.\n");
    else {
        LOG("failed!\n");
        exit(1);
    }
    
    LOG("Attempting to serve... ");
    
    sipc = IPCF_SERVE(ipcdata, 10);
    
    while(sipc->status == IPCSTAT_PREPARING);
        
    LOG("status %d (errno %d) ", sipc->status, sipc->errn);
    
    if (sipc->status == IPCSTAT_SERVING)
        LOG("ok! Listening on port %d.\n", ipcdata->sdata.addr.sin_port);
        
    else {
        LOG("failed!\n");
        exit(1);
    }
    
    if (pid = fork()) {
        LOG("Server: pid %d.\n", spid = getpid());
        
        while(!stop) {
        
            fflush(stdout);
            if (temp = recvMessage(sipc)) {
                LOG("(%d) Server received: ", spid);
                mprintln(temp);
                
                /* Swap from and to, so that the message is routed: */
                
                temp->header.to = temp->header.from;
                temp->header.from = 0;

                sendMessage(sipc, temp);
                LOG("(%d) Server sent: ", spid);
                mprintln(temp);

                mdel(temp);    
            }
        }
        
    } else {
    
	    LOG("Creating IPCData for client... "); // Macro defined in tools.h
	    
    	if ((ipcdata2 = IPCF_IPCDATA(IPCF_IPCDATA_ARGS)) != NULL)
		    LOG("ok.\n");
		else {
		    LOG("failed!\n");
		    exit(1);
		}
    
        LOG("Client: pid %d.\n", cpid = getpid());
        LOG("Attempting to connect... ");

        cipc = IPCF_CONNECT(ipcdata2, 1);

        while (cipc->status == IPCSTAT_CONNECTING);
        
        LOG("status %d (errno %d) ", cipc->status, cipc->errn);

        if (cipc->status == IPCSTAT_CONNECTED)
            LOG("ok!\n");
            
        else {
            LOG("failed!\n");
            exit(1);
        }

        sendMessage(cipc, temp = mnew(1, 0, 5, "ping!"));
        mdel(temp);
        
        while(!stop) {
        
            if (temp = recvMessage(cipc)) {

                LOG("(%d) Client received: ", spid);
                mprintln(temp);
                
                /* Bounce the message! */
                temp->header.to = temp->header.from;
                temp->header.from = 1;
                
                LOG("(%d) Client sent: ", spid);
                mprintln(temp);
                sendMessage(cipc, temp);
                mdel(temp);
                
            }
            
        }
    }
}
