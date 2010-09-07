#include "../include/ipc_queue.h"

/*I was 2*/
/*I am CLIENT*/

int main() {

	int n;
	message_t msg;
	char mtext[200];
	ipc_t ipc;
	ipcdata_t ipcdata;
	
	ipcdata = mq_ipcData(CLIENTSENDPRIOR, CLIENTRECVPRIOR);
	
	ipc = mq_connect(ipcdata);
	
	while((n = read(0,mtext,sizeof mtext)) > 0)
	{
		msg = mnew(CLIENTSENDPRIOR,CLIENT2RECVPRIOR,n,mtext);
		printf("Cliente envia: %.*s", n, mdata(msg));
		qput(ipc->outbox,msg);
		
		
		msg = qget(ipc->inbox);
		printf("enviado");
		printf("%d\n", msg == NULL);
		if(msg != NULL)
		{
			printf("Cliente recibe: %.*s", mdlen(msg), mdata(msg));
		}
		fflush(stdout);
	}
	printf("Cliente termina\n");
	mdel(msg);
	free(ipc);
	return 0;

	/* este anda */
	
	/*int n;
	message_t msg;
	
	char mtext[200];
	
	ipc_t ipc;
	
	ipc = mq_connect(2,2);
	
	while ( (n = read(0, mtext, sizeof mtext)) > 0 )
	{
		msg = mnew(2,1,n,mtext);
		printf("Cliente envia: %.*s", n, mdata(msg));
		//msgsnd(qout, &msg, n, 0);
		mq_sendData(ipc,msg,1);
		//n = msgrcv(qin, &msg, sizeof msg.mtext, msg.mtype, 0);
		msg = mq_getData(ipc, 2);
		if ( msg > 0 )
			printf("Cliente recibe: %.*s", mdlen(msg), mdata(msg));
	}
	printf("Cliente termina\n");
	mdel(msg);
	free(ipc);
	return 0;*/
	
	/*El de arriba anda*/
	
	/*int n;
	message_t msg;
	
	char mtext[200];
	
	ipc_t ipc;
	
	ipc = mq_connect();
	
	while ( (n = read(0, mtext, sizeof mtext)) > 0 )
	{
		msg = mnew(2,1,n,mtext);
		printf("Cliente envia: %.*s", n, mdata(msg));
		//msgsnd(qout, &msg, n, 0);
		mq_sendData(ipc,msg);
		//n = msgrcv(qin, &msg, sizeof msg.mtext, msg.mtype, 0);
		msg = mq_getData(ipc, 2);
		if ( msg > 0 )
			printf("Cliente recibe: %.*s", mdlen(msg), mdata(msg));
	}
	printf("Cliente termina\n");
	mdel(msg);
	free(ipc);
	return 0;*/
}
