#include "../include/ipc.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#define QKEY	(key_t)0105
#define QPERM	0660
#define MAXOBN	50
