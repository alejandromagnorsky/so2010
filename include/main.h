/*
**  SISTEMAS OPERATIVOS, 2C 2010
**  PRIMER TRABAJO PRACTICO
**  Simulacion de una colonia de hormigas.

**  File:       main.h
**  Content:    main executable header.
*/

#ifndef __MAIN_H__
#define __MAIN_H__

/* Before including any other headers, some definitions are needed: */

#define LOGGING_ENABLED
#define IPC_METHOD_SOCKETS

//#define IPC_METHOD_MQS
//#define IPC_METHOD_FIFOS
//#define IPC_METHOD_SHMEM

#ifdef IPC_METHOD_SOCKETS
    #include "ipc_sock.h"
    struct sockaddr_in DEFAULT_ADDR = {AF_INET, 4548, INADDR_ANY};

    // Requirements to build IPCData vary from IPC method to IPC method
    #define IPCF_IPCDATA(...)   sockIPCData(__VA_ARGS__)
    #define IPCF_IPCDATA_ARGS   &DEFAULT_ADDR
    
    // IPCF_SERVE always receives ipcdata_t and the max. number of clients
    #define IPCF_SERVE(X, Y)    sockServe((X), (Y))

    // IPCF_CONNECT always receives ipcdata_t
    #define IPCF_CONNECT(X)     sockConnect(X)
#endif

#ifdef IPC_METHOD_MQS
	#include "ipc_queue.h"
	
	// Requirements to build IPCData vary from IPC method to IPC method
    #define IPCF_IPCDATA(...)   mq_ipcdata(__VA_ARGS__)
    #define IPCF_IPCDATA_ARGS   (getpid(), getpid())
    
    // IPCF_SERVE always receives ipcdata_t
    #define IPCF_SERVE(X, Y)    mq_serve(X)

    // IPCF_CONNECT always receives ipcdata_t
    #define IPCF_CONNECT(X)     mq_connect(X)
#endif

#include "tools.h"
#include "ipc.h"

#endif // Include guard endif

ipc_t initServer();
ipc_t initClient();
