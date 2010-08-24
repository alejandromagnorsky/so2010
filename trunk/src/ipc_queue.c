#include "../include/ipc_queue.h"

ipc_t mq_connect()
{
	int queue_id;
	
	ipc_t newIpc = malloc(sizeof(struct st_ipc_t));
	
	newIpc->ipcdata = malloc(sizeof(union un_ipcdata_t));
	
	queue_id = init_queue();
	
	if(queue_id == -1)
	{
		newIpc->status = -1;
		return newIpc;
	}
	
	newIpc->status = 0;
	newIpc->ipcdata->queuedata.id = queue_id;
	return newIpc;
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

	if(priority > MAXPRIOR || priority< 0)
	{
		warn("invalid priority level");
		return (-1);
	}
	
	if(queue_id = init_queue() == -1)
	{
		return (-1);
	}
	
	s_entry.mtype = (long)priority;
	strncpy(s_entry.mtext, msg->data, MAXOBN);

	if(msgsnd(ipc->ipcdata->queuedata.id, &s_entry, msg->len,0) == -1){
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
		return mnew(0,0,0,"");
	}
	
	//while(1){
		if((mlen = msgrcv(ipc->ipcdata->queuedata.id, &r_entry, MAXOBN,priority, MSG_NOERROR)) == -1){
			perror("msgrcv failed");
			return mnew(0,0,0,"");
		}
		else {
			r_entry.mtext[mlen] = '\0';
			msg = mnew(0,0,mlen,r_entry.mtext);
			return msg;
		}
	//}
	
}

int mq_disconnect(ipc_t ipc)
{
	/* existe? */
}

int warn(char * s)
{
	fprintf(stderr, "warning: %s\n", s);
}

