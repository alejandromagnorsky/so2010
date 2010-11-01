#include "../include/shell.h"

#define NUM_COMMANDS 15

#define SHELL_PROMPT "SuciOS_tty%d$ "

static struct {
    char* name;
    char* description;
    int (*function) (char*);
} commands[NUM_COMMANDS] =  {
                 {"divzero", "Raise a zero divide exception", divideByZero},
                 {"cpuid", "Display CPU information", detect_cpu},
                 {"help", "Display system commands", help},
		         {"grprot", "Raise a general protection Exception", gralprotection},
		         {"echo", "Prints string", echo},
		         {"startx", "Start graphic OS", startx},
		         {"clear", "Clear the screen", clear},
		         {"top", "Shows active processes and statistics", top},
		         {"daemontest", "Queries the echo and reverse echo system servers", daemontest},
				 {"demo_malloc", "demo malloc", demo_malloc},
				 {"doNothing", "Runs a task that doesn't end 4 times as children", doNothing4Times},
				 {"kill", "kills a task with the id you give and its children", kill},
                 {"echoserver", "A server that echoes messages sent to it", echoserver},
                 {"ohceserver", "ti ot tnes segassem seohce taht revres A", ohceserver},
				 {"doGetChar", "Makes a getchar to try running in background", do_getchar}
};


int get_command(command_t* command){
	linebuffer_t bufferinput = {{0},0};

	getting_command(&bufferinput);
	putchar('\n');
	parse_command(&bufferinput, command) ;
	bufferinput.line[0] = bufferinput.pos = 0;
	run_command(command);
}

int demo_malloc(char * a){
	int qty = 1000;
	while(qty--){
		void * a = malloc(100);
		printf("%d ", (unsigned int) a);
	}
	printf("\n");
}

void getting_command(linebuffer_t* bufferinput){
	unsigned char c;	
	while( (c = getchar()) != '\n' && bufferinput->pos < (LINEBUF_LEN - 1) ){
		
		switch(c){
			case '\b':
				if(bufferinput->pos > 0){
					bufferinput->pos--;
					putchar(c);
				}
				break;
			default:
				bufferinput->line[bufferinput->pos++] = c;
				putchar(c);
				break;
		}		
	}
	bufferinput->line[bufferinput->pos] = '\0';
}

void parse_command(linebuffer_t* linebuffer, command_t* command) {
    int initpos = 0;
    while ( (linebuffer->line[initpos] == ' ') && (++initpos < LINEBUF_LEN - 1) );
    sscanf(linebuffer->line + initpos, "%s %s", command->name, command->args);
}

void input_handler(){

}

int run_command(command_t* command){
	int i = 0, ret, bk = 0;
	char * bkstring = "&";
    
	if(streq(command->name, ""))
		return 0;
	
	while(command->args[i++] != 0);
	if(command->args[i - 2] == '&'){
		bk = 1;
	}
	
	for (i = 0; i < NUM_COMMANDS; i++) {
	    if(streq(command->name, commands[i].name)) {
			if(bk){
				ret = System.execb(commands[i].function, command->args);			
			}else{
				ret = System.exec(commands[i].function, command->args);			
				System.wait();
			}
			clearCommand(command);
            return ret;
	    }
	}

	printf("%s: command not found\n", command->name);	
	clearCommand(command);
	return 1;
}

void clearCommand(command_t* command){
	command->name[0] = 0;
	command->args[0] = 0;
}

int divideByZero(char* line){
	int x = 1;
	int y = 1;
	int z = x / --y;
	
	return 0; /* Not happening anyway */
}

int echo(char* text)
{
	printf(text);
	printf("\n");
	return 0;
}


int help(char* line) {
    int i;
    printf("Available commands: \n");
    
	for (i = 0; i < NUM_COMMANDS; i++)
	    printf("%s\t\t%s\n", commands[i].name, commands[i].description);
	printf("\n");
	return 0;
}

int startx(char* line){
	printf("This computer does not meet the requirements for the graphical interface\n");
	printf("Requirements:\n");
	printf("\t5TB of system memory.\n");
	printf("\t3TB of video memory.\n");
	printf("\t800TB of free space in hard disk.\n");
	printf("\n");
	return 0;
}

int clear(char* line) {
    int i;
    for (i = 0; i < VIDEO_SIZE; i++)
		printf(" ");
        //System.device[DEVICE_SCREEN]->driver->write(DEVICE_SCREEN, " ", 1);
        
    return 0;
}

int top (char* line) {
    System.name("top");
    
    printf("Processes and it's CPU percentage of use\n");
    task_t task;
    int iter = 0, self, first, other;
    int rank, prio, usage, status, rm;
    char name[MAX_TASK_NAME];
    
    self = System.gettid();

    other = first = System.nexttid(&iter);
    
    do {
        rank = System.getrank(other);
        prio = System.getprio(other);
        usage = System.getcpuc(other);
		System.getName(name, other);
        status = System.getStatus(other);
        rm = System.getRMode(other);

        task = Task.getByTID(other);
        // [TODO] change name for a sys call
        printf("TID: %d\tPriority: %d\tRank: %d\tUsage: %d\t RunningMode: %d\tName: %s (%d)\n", 
        		other, rank, prio, usage, rm, name, status);
        other = System.nexttid(&iter);
    } while (first != other);
}

int kill(char* tid)
{
	System.name("kill");
	int tidi, status;
	tidi = atoi(tid,10);
	status = System.kill(tidi);
	
	switch(status)
	{
		case KILL_INVALID_TASK:
			printf("The given task doesnt exist, you should pass as an argument an existing task id\n");
			break; 
		case KILL_PERMISSION_DENIED:
			printf("Permission denied\n");
			break;
		case KILL_SUCCEEDED:
			printf("Task %d killed\n", tidi);
			break;
	}
	return 0;
}

int echoserver(char* line) {

    int len, from;
    char msg[128];

    System.name("echoserver");
    
    while(1) {
        len = System.recv();
        from = System.getmsg(msg, len);
        
        System.clsmsg();

        System.send(from, msg, len);
    }

}

int ohceserver(char* line) {
    int len, from, i;
    char msg[128], reverse[128];

    System.name("ohceserver");
    
    while(1) {
        len = System.recv();
        from = System.getmsg(msg, len);

        for(i = 0; i < len; i++)
            reverse[i] = msg[len - i - 1];
        
        System.clsmsg();

        System.send(from, reverse, len);
    }    
}

int do_getchar(char* line){
	System.name("doGetChar");
	printf("Press a key to terminate the program.\n");
	getchar();
	printf("Thanks!!\n");
}

int daemontest(char * line) {

    int len, to;
    char msg[128];
    System.name("daemontest");

    /* System.exec(task3, "3");
    System.sleep(100);
    System.exec(task2, "2");
  */
    printf("Sending 'prueba' to echo server...\n");

    if (to = System.gettid("echoserver"))
        System.send(to, line, strlen(line));
    else
        printf("Echo server not running.\n");

    len = System.recv();
    System.getmsg(msg, len);

    msg[len] = 0;

    printf("Got: %s\n", msg);

    printf("Sending 'prueba' to reverse echo server...\n");
    
    if (to = System.gettid("ohceserver")) {
        System.send(to, line, strlen(line));
    } else
        printf("Reverse echo server not running.\n");
    
    len = System.recv();
    System.getmsg(msg, len);

    msg[len] = 0;

    printf("Got: %s\n", msg);
}

void init() {

    System.exec(echoserver, "");
    System.exec(ohceserver, "");

    return;
}
