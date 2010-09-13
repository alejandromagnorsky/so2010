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

#define IPC_METHOD_SOCKETS
//#define IPC_METHOD_MQS
//#define IPC_METHOD_FIFOS
//#define IPC_METHOD_SHMEM

#ifdef IPC_METHOD_SOCKETS
    #include "ipc_sock.h"
    #define IPC_METHOD "SOCKETS"
    struct sockaddr_in DEFAULT_ADDR = {AF_INET, 4549, INADDR_ANY};

    // Requirements to build IPCData vary from IPC method to IPC method
    #define IPCF_IPCDATA(...)   sockIPCData(__VA_ARGS__)
    #define IPCF_IPCDATA_ARGS   &DEFAULT_ADDR
    
    // IPCF_SERVE always receives ipcdata_t and the max. number of clients
    #define IPCF_SERVE(X, Y)    sockServe((X), (Y))

    // IPCF_CONNECT always receives ipcdata_t
    #define IPCF_CONNECT(X,Y)     sockConnect(X, Y)
#endif

#ifdef IPC_METHOD_MQS
	#include "ipc_queue.h"
	#define IPC_METHOD "MQS"
	
	// Requirements to build IPCData vary from IPC method to IPC method
    #define IPCF_IPCDATA(...)   mq_ipcdata(__VA_ARGS__)
    #define IPCF_IPCDATA_ARGS   getpid(), getpid()
    
    // IPCF_SERVE always receives ipcdata_t
    #define IPCF_SERVE(X, Y)    mq_serve(X)

    // IPCF_CONNECT always receives ipcdata_t
    #define IPCF_CONNECT(X,Y)     mq_connect(X)
#endif
    
    
#ifdef IPC_METHOD_FIFOS
	#include "ipc_fifo.h"
	#define IPC_METHOD "FIFOS"
	
	// Requirements to build IPCData vary from IPC method to IPC method
    #define IPCF_IPCDATA(...)   fifoIPCData(__VA_ARGS__)
    #define IPCF_IPCDATA_ARGS   sid
    
    // IPCF_SERVE always receives ipcdata_t
    #define IPCF_SERVE(IPCDATA, QTYANTS)   fifoServe(QTYANTS)

    // IPCF_CONNECT always receives ipcdata_t
    #define IPCF_CONNECT(IPCDATA, ANTID)     fifoConnect(IPCDATA)
#endif
    

#ifdef IPC_METHOD_SHMEM
	#include "ipc_shm.h"
	#define IPC_METHOD "SHM"
	
	// Requirements to build IPCData vary from IPC method to IPC method
    #define IPCF_IPCDATA(...)   shmIPCData()
    #define IPCF_IPCDATA_ARGS   
    
    // IPCF_SERVE always receives ipcdata_t
    #define IPCF_SERVE(IPCDATA, QTYANTS)   shmServe()

    // IPCF_CONNECT always receives ipcdata_t
    #define IPCF_CONNECT(IPCDATA, ANTID)     shmConnect(IPCDATA, ANTID)
#endif

#include "io.h"
#include "tools.h"
#include "ipc.h"
#include "message.h"
#include "cmd.h"

#endif // Include guard endif

ipc_t initServer();
ipc_t initClient();
