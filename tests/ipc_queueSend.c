#include "../include/ipc_queue.h"

/*I am 2*/

int main() {

	int n;
	message_t msg;
	
	char mtext[200];
	
	ipc_t ipc;
	
	ipc = mq_connect();
	
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
	return 0;
	
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
