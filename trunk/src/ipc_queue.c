#include "../include/ipc_queue.h"

ipc_t mq_connect()
{
	int queue_id;
	int rcreat;
	
	ipc_t newIpc = malloc(sizeof(struct st_ipc_t));
	newIpc->status = IPCSTAT_DISCONNECTED;
	
	newIpc->ipcdata = malloc(sizeof(union un_ipcdata_t));
	
	queue_id = init_queue();
	newIpc->status = IPCSTAT_CONNECTING;
	
	pthread_create(&(newIpc->thread), NULL, mq_serverLoop, newIpc);
	
	if(queue_id == -1)
	{
		newIpc->status = IPCERR_MSGGETFAILED;
		return newIpc;
	}
	
	newIpc->stop = 0;
	newIpc->status = IPCSTAT_CONNECTED;
	newIpc->ipcdata->queuedata.id = queue_id;
	return newIpc;
}

void * mq_serverLoop(void* ipcarg)
{
	ipc_t ipc;
	int i;
	ipc = (ipc_t) ipcarg;
	struct st_sclient_t* clts;
	message_t msg;
	
	clts = (struct st_sclient_t*) calloc (ipc->maxclts,
                                         sizeof(struct st_sclient_t));
    
    while (!(ipc->stop)) {
        
        
        /*hacer esta parte*/
        msg = qget(ipc->outbox);
        
        if(msg != NULL)
        {
        	mq_sendData(ipc, msg);
        }
        
        msg = mq_getData(ipc, priority);
        
        qput(ipc->inbox, msg);
    
    }
    
    for (i = 0; i < ipc->maxclts; i++)
    {
        if (clts[i].active)
        {
        	close(clts[i].fd);
        }
    }
    
    mq_disconnect(ipc);
    
    return;
}

int init_queue(void)
{
	int queue_id;
	
	if((queue_id = msgget(QKEY, IPC_CREAT | QPERM)) == -1)
	{
		perror("msgget failed");
	}
	
	return queue_id;
}

int mq_sendData(ipc_t ipc, message_t msg)
{
	int priority = msg->header.to;
	int queue_id;
	struct q_entry s_entry;

	if(priority > MAXPRIOR || priority< 0)
	{
		ipc->status = IPCERR_INVALIDPRIORITY;
		warn("invalid priority level");
		return (-1);
	}
	
	if(queue_id = init_queue() == -1)
	{
		ipc->status = IPCERR_MSGGETFAILED;
		return (-1);
	}
	
	s_entry.mtype = (long)priority;
	strncpy(s_entry.mtext, mdata(msg), MAXOBN);

	if(msgsnd(ipc->ipcdata->queuedata.id, &s_entry, 
								mdlen(msg),MSG_NOERROR) == -1){
		ipc->status = IPCERR_MSGSNDFAILED;
		perror("msgsnd failed");
		return (-1);
	} 
	else {
		return (0);
	}
}

message_t mq_getData(ipc_t ipc, int priority)
{
	int mlen, r_qid;
	message_t msg;
	struct q_entry r_entry;
	
	int queue_id;

	if((r_qid = init_queue()) == -1)
	{
		ipc->status = IPCERR_MSGGETFAILED;
		return mnew(0,0,0,"");
	}
	
	//while(1){
		if((mlen = msgrcv(ipc->ipcdata->queuedata.id, &r_entry, 
									MAXOBN, priority, MSG_NOERROR)) == -1){
			ipc->status = IPCERR_MSGRCVFAILED;
			perror("msgrcv failed");
			return mnew(0,0,0,"");
		}
		else {
			//r_entry.mtext[mlen] = '\0';
			msg = mnew(0,priority,mlen,r_entry.mtext);
			return msg;
		}
}

void mq_disconnect(ipc_t ipc)
{
	msgctl(ipc->ipcdata->queuedata.id,IPC_RMID, NULL);
	return;
}

int warn(char * s)
{
	fprintf(stderr, "warning: %s\n", s);
}

