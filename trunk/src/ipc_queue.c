#include "../include/ipc_queue.h"

ipc_t mq_connect(int sendprior, int recvprior)
{
	int queue_id;
	
	ipc_t newIpc = (ipc_t)malloc(sizeof(struct st_ipc_t));
	newIpc->status = IPCSTAT_DISCONNECTED;
	
	newIpc->ipcdata = malloc(sizeof(union un_ipcdata_t));
	
	queue_id = init_queue();
	newIpc->status = IPCSTAT_CONNECTING;
	
	if(queue_id == -1)
	{
		newIpc->status = IPCERR_MSGGETFAILED;
		return newIpc;
	}
	
	newIpc->inbox = qnew();
	newIpc->outbox = qnew();
	newIpc->stop = 0;
	newIpc->status = IPCSTAT_CONNECTED;
	newIpc->ipcdata->queuedata.id = QKEY;
	newIpc->ipcdata->queuedata.sendPrior = sendprior;
	newIpc->ipcdata->queuedata.recvPrior = recvprior;
	
	newIpc->thread = pthread_create(&(newIpc->thread), NULL, mq_clientLoop, newIpc);
	
	return newIpc;
}

ipc_t mq_serve(int sendprior, int recvprior)
{
	int queue_id;
	
	ipc_t newIpc = (ipc_t)malloc(sizeof(struct st_ipc_t));
	newIpc->status = IPCSTAT_DISCONNECTED;
	
	newIpc->ipcdata = malloc(sizeof(union un_ipcdata_t));
	
	queue_id = init_queue();
	newIpc->status = IPCSTAT_CONNECTING;
	
	if(queue_id == -1)
	{
		newIpc->status = IPCERR_MSGGETFAILED;
		return newIpc;
	}
	
	newIpc->inbox = qnew();
	newIpc->outbox = qnew();
	newIpc->stop = 0;
	newIpc->status = IPCSTAT_CONNECTED;
	newIpc->ipcdata->queuedata.id = QKEY;
	newIpc->ipcdata->queuedata.sendPrior = sendprior;
	newIpc->ipcdata->queuedata.recvPrior = recvprior;
	
	newIpc->thread = pthread_create(&(newIpc->thread), NULL, mq_serverLoop, newIpc);
	
	return newIpc;
}

void * mq_serverLoop(void* ipcarg)
{
	ipc_t ipc;
	int i;
	ipc = (ipc_t) ipcarg;
	message_t msg;
	
	if(ipc->status == IPCSTAT_DISCONNECTED)
	{
		return;
	}
	
	ipc->status = IPCSTAT_SERVING;
	
    while (ipc->stop != 1) {
    	
        msg = mq_getData(ipc, SERVERKEY);
        if(msg != NULL)
        {
			if(msg->header.to == 0)
		    {
		    /*
		    	//[TODO] ver que pasa si hay un mensaje con to = 0
		    	//pero hay que cambiar el to.. 0 no existe
		    	
		    	/* Es esto lo que tiene que hacer?
		    	qput(ipc->inbox, msg);*/
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
	
	if(ipc->status == IPCSTAT_DISCONNECTED)
	{
		return;
	}
	
	ipc->status = IPCSTAT_SERVING;
	
    while (ipc->stop != 1) {
    	
    	msg = mq_getData(ipc, ipc->ipcdata->queuedata.recvPrior);
    	if(msg != NULL)
    	{
			qput(ipc->inbox, msg);
    	}
    	
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

int init_queue(void)
{
	int queue_id;
	
	if((queue_id = msgget(QKEY, IPC_CREAT | QPERM)) == -1)
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
	
	if(queue_id = init_queue() == -1)
	{
		ipc->status = IPCERR_MSGGETFAILED;
		return (-1);
	}
	
	s_entry.mtype = (long)priority;
	data = mserial(msg);
	memcpy(s_entry.mtext, data, strlen(data));

/*	msg = NULL;
	msg = mdeserial(s_entry.mtext);
	mprintln(msg);
	printf("type: %ld\n",s_entry.mtype);
	printf("qkey: %d\n",(int)QKEY);
	printf("maxobn: %d\n",MAXOBN);
	fflush(stdout);*/
	
	if(msgsnd(QKEY, &s_entry, strlen(data),MSG_NOERROR) == -1)
	{
		ipc->status = IPCERR_MSGSNDFAILED;
		/*msg = NULL;
		msg = mdeserial(s_entry.mtext);
		mprintln(msg);
		printf("type: %ld\n",s_entry.mtype);
		printf("qkey: %d\n",(int)QKEY);*/
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
		return NULL;
	}
	
	if((mlen = msgrcv(QKEY, &r_entry, MAXOBN, (long)priority, MSG_NOERROR)) == -1){
		/*ipc->status = IPCERR_MSGRCVFAILED;
		perror("msgrcv failed");*/
		return NULL;
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
	msgctl(QKEY,IPC_RMID, NULL);
	free(ipc->ipcdata);
	qdel(ipc->inbox);
	qdel(ipc->outbox);
	free(ipc);
	return;
}

int warn(char * s)
{
	fprintf(stderr, "warning: %s\n", s);
}

