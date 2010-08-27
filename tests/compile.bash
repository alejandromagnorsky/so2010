#!/usr/bin/bash

MESSAGE_REQS="../src/message.c ../src/tools.c"
QUEUE_REQS="$MESSAGE_REQS ../src/queue.c"

gcc message_test.c $MESSAGE_REQS -o message_test
gcc queue_singletest.c $QUEUE_REQS -o queue_singletest
