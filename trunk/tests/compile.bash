#!/bin/bash

##
##  SISTEMAS OPERATIVOS, 2C 2010
##  PRIMER TRABAJO PRACTICO
##  Simulacion de una colonia de hormigas.

##  File:       compile.bash
##  Content:    bash script to compile the tests.
##

MESSAGE_DEP="../src/message.c"
QUEUE_DEP="../src/message.c ../src/queue.c"
ANYIPC_DEP="../src/message.c ../src/queue.c ../src/ipc.c ../src/ipc_sock.c ../src/tools.c ../src/ipc_fifo.c ../src/ipc_queue.c"

#gcc $MESSAGE_DEP message_test.c -o message_test
#gcc $QUEUE_DEP queue_singletest.c -o queue_singletest
gcc $ANYIPC_DEP anyipc_test.c -lpthread -o anyipc_test -g
