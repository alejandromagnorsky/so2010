/*
**  SISTEMAS OPERATIVOS, 2C 2010
**  PRIMER TRABAJO PRACTICO
**  Simulacion de una colonia de hormigas.

**  File:       sock_test.c
**  Content:    tests for the socket IPC library.
*/

#include <stdio.h>
#include "../include/sock.h"


int main() {

    pid_t pid;
    message_t temp;
    ipcdata_t ipcdata;
    ipc_t sipc, cipc;
    
    char buffer[100];

    int count = 0;
    char stop = 0;
    
    printf("Creating IPCData... ");
    
    if ((ipcdata = sockIPCData()) != NULL)
        printf("ok.\n");
    else {
        printf("failed!");
        exit(1);
    }
    
    printf("Attempting to serve... ");
    
    sipc = sockServe(ipcdata, 10);
    
    while(sipc->status == IPCSTAT_PREPARING);
        
    printf("status %d (errno %d) ", sipc->status, sipc->errn);
    
    if (sipc->status == IPCSTAT_SERVING)
        printf("ok! Listening on port %d.\n", ipcdata->sdata.addr.sin_port);
        
    if (pid = fork()) {
    
        while(!stop) {
            fflush(stdout);
            if (temp = qget(sipc->inbox)) {
            

                temp->header.to = temp->header.from;
                temp->header.from = 0;

                qput(sipc->outbox, temp);
                mdel(temp);
                
            }
        }
        
    } else {
        printf("Attempting to connect... ");

        cipc = sockConnect(ipcdata);

        while (cipc->status == IPCSTAT_CONNECTING);
        
        printf("status %d (errno %d) ", cipc->status, cipc->errn);
        
        if (cipc->status == IPCSTAT_CONNECTED);
            printf("ok!\n");

        qput(cipc->outbox, mnew(1, 0, 5, "c2s-1"));
        qput(cipc->outbox, mnew(1, 0, 5, "c2s-2"));
        qput(cipc->outbox, mnew(1, 0, 5, "c2s-3"));
        
        while(!stop) {

            if (temp = qget(cipc->inbox)) {
                
                printf("Client Got: ");
                mprintln(temp);
                temp->header.to = temp->header.from;
                temp->header.from = 1;
                printf("Client Sent: ");
                qput(cipc->outbox, temp);
                mdel(temp);
                
            }
            
        }
    }
}
