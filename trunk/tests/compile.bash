#!/bin/bash

MESSAGE_REQS="../src/message.c ../src/tools.c"
QUEUE_REQS="$MESSAGE_REQS ../src/queue.c"

MSGQUEUE_REQS="../src/ipc_queue.c"

gcc message_test.c $MESSAGE_REQS -o message_test
#gcc queue_singletest.c $QUEUE_REQS -o queue_singletest

gcc -pthread -g ipc_queueClient2.c $QUEUE_REQS $MSGQUEUE_REQS -o msgqueue_client2
gcc -pthread -g ipc_queueClient.c $QUEUE_REQS $MSGQUEUE_REQS -o msgqueue_client
gcc -pthread -g ipc_queueServer.c $QUEUE_REQS $MSGQUEUE_REQS -o msgqueue_server
