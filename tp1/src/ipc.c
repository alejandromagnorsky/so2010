/*
**  SISTEMAS OPERATIVOS, 2C 2010
**  PRIMER TRABAJO PRACTICO
**  Simulacion de una colonia de hormigas.

**  File:       ipc.c
**  Content:    functions for the IPC abstract interface.
*/

#include "../include/ipc.h"

int sendMessage(ipc_t ipc, message_t m) {
    return qput(ipc->outbox, m);
}

message_t recvMessage(ipc_t ipc) {
    return qget(ipc->inbox);
}
