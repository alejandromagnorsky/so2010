#include "../include/ipc_queue.h"

ipc_t mq_connect(int key)
{
	int queue_id;
	int rcreat;
	
	ipc_t newIpc = malloc(sizeof(struct st_ipc_t));
	newIpc->status = IPCSTAT_DISCONNECTED;
	
	newIpc->ipcdata = malloc(sizeof(union un_ipcdata_t));
	
	queue_id = init_queue(key);
	newIpc->status = IPCSTAT_CONNECTING;
	
	pthread_create(&(newIpc->thread), NULL, mq_clientLoop, newIpc);
	
	if(queue_id == -1)
	{
		newIpc->status = IPCERR_MSGGETFAILED;
		return newIpc;
	}
	
	newIpc->inbox = qnew();
	newIpc->outbox = qnew();
	newIpc->stop = 0;
	newIpc->status = IPCSTAT_CONNECTED;
	newIpc->ipcdata->queuedata.id = queue_id;
	return newIpc;
}

ipc_t mq_serve(int key)
{
	int queue_id;
	int rcreat;
	
	ipc_t newIpc = malloc(sizeof(struct st_ipc_t));
	newIpc->status = IPCSTAT_DISCONNECTED;
	
	newIpc->ipcdata = malloc(sizeof(union un_ipcdata_t));
	
	queue_id = init_queue(key);
	newIpc->status = IPCSTAT_CONNECTING;
	
	pthread_create(&(newIpc->thread), NULL, mq_serverLoop, newIpc);
	
	if(queue_id == -1)
	{
		newIpc->status = IPCERR_MSGGETFAILED;
		return newIpc;
	}
	
	newIpc->inbox = qnew();
	newIpc->outbox = qnew();
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
	message_t msg;
	
    while (!(ipc->stop)) {
    	
        msg = mq_getData(ipc, SERVERKEY);  //[TODO] cambiar el SERVERPRIOR por la prioridad que tenga el servidor
        if(msg != NULL)
        {
		    if(msg->header.to == 0)
		    {
		    	//[TODO] ver que pasa si hay un mensaje con to = 0
		    	//pero hay que cambiar el to.. 0 no existe
		    	
		    	/* Es esto lo que tiene que hacer? */
		    	qput(ipc->inbox, msg);
		    }
		    else 
		    {
		    	mq_sendData(ipc,msg,msg->header.to);
		    }
        }
    }
    
    mq_disconnect(ipc);
    free(msg);
    return;
}

void * mq_clientLoop(void* ipcarg)
{
	ipc_t ipc;
	int i;
	ipc = (ipc_t) ipcarg;
	message_t msg;
	
    while (!(ipc->stop)) {
    	
    	msg = mq_getData(ipc, ipc->ipcdata->queuedata.id);
    	qput(ipc->inbox, msg);
    	
    	msg = qget(ipc->outbox);
        if(msg != NULL)
        {
        	mq_sendData(ipc,msg,SERVERKEY);
        }
    }
    
    mq_disconnect(ipc);
    free(msg);
    return;
}

int init_queue(int key)
{
	int queue_id;
	
	if((queue_id = msgget((key_t)key, IPC_CREAT | QPERM)) == -1)
	{
		perror("msgget failed");
	}
	
	return queue_id;
}

int mq_sendData(ipc_t ipc, message_t msg, int priority)
{
	int queue_id;
	struct q_entry s_entry;
	char * data;

	if(priority > MAXPRIOR || priority< 0)
	{
		ipc->status = IPCERR_INVALIDPRIORITY;
		warn("invalid priority level");
		return (-1);
	}
	
	if(queue_id = init_queue(ipc->ipcdata->queuedata.id) == -1)
	{
		ipc->status = IPCERR_MSGGETFAILED;
		return (-1);
	}
	
	s_entry.mtype = (long)priority;
	data = mserial(msg);
	memcpy(s_entry.mtext, data, MAXOBN);

	if(msgsnd(ipc->ipcdata->queuedata.id, &s_entry, 
								mfsize(msg),MSG_NOERROR) == -1){
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

	if((r_qid = init_queue(ipc->ipcdata->queuedata.id)) == -1)
	{
		ipc->status = IPCERR_MSGGETFAILED;
		return mnew(0,0,0,"");
	}
	
	if((mlen = msgrcv(ipc->ipcdata->queuedata.id, &r_entry, 
								MAXOBN, priority, MSG_NOERROR)) == -1){
		ipc->status = IPCERR_MSGRCVFAILED;
		perror("msgrcv failed");
		return mnew(0,0,0,"");
	}
	else {
		//r_entry.mtext[mlen] = '\0';
		/*msg = mnew(0,priority,mlen,r_entry.mtext);
		return msg;*/
		msg = mdeserial(r_entry.mtext);
		return msg;
	}
}

void mq_disconnect(ipc_t ipc)
{
	msgctl(ipc->ipcdata->queuedata.id,IPC_RMID, NULL);
	free(ipc);
	return;
}

int warn(char * s)
{
	fprintf(stderr, "warning: %s\n", s);
}

