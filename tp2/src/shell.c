#include "../include/shell.h"

#define NUM_COMMANDS 9

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
		         {"testTasks", "Tests tasks by creating 3, killing one and ending the other 2", testTasks}
};


void shell(int tty) {

	linebuffer_t linebuffer = {{0}, 0};
	command_t command;

    static char show_prompt = 1;
	unsigned char a;
    int exit_status,i;
    
    if (show_prompt)
        printf(SHELL_PROMPT, tty);

	switch(a = getchar()) {
	
		case '\n':
		    /* Print the newline and finalize the linebuffer string with 0: */
			putchar('\n');
			linebuffer.line[linebuffer.pos] = 0;
		    
		    parse_command(linebuffer, command);
		    
			/* Once parsing is over, reset the linebuffer and shell status: */
		    show_prompt = 1;
			linebuffer.line[0] = linebuffer.pos = 0;
			
			exit_status = run_command(command);
			
			break;
			
		case '\b':
    		show_prompt = 0; /* Do not reprint prompt after backspace */
			if(linebuffer.pos != 0) {
				linebuffer.line[linebuffer.pos--] = 0x00;				
				putchar('\b');
			}
			break;
			
		default:
    		show_prompt = 0; /* Do not reprint prompt after character entry */		
			if(linebuffer.pos < LINEBUF_LEN - 1) {
				linebuffer.line[linebuffer.pos++] = a;
				putchar(a);
			}
			
			break;
	}
}

void parse_command(linebuffer_t linebuffer, command_t command) {
    int initpos = 0;
    
    /* Remove preceding whitespaces: */
    while ( (linebuffer.line[initpos] == ' ') && (++initpos < LINEBUF_LEN - 1) );
    sscanf(linebuffer.line + initpos, "%s %s", command.name, command.args);
}

void input_handler(){

}

int run_command(command_t command){
	int i;
	if(streq(command.name, ""))
		return 0;
		
	for (i = 0; i < NUM_COMMANDS; i++) {
	    if (streq(command.name, commands[i].name)) {
	        clearCommand(command);
	        return System.exec(commands[i].function, command.args);
	    }
	}

	printf("%s: command not found\n", command.name);	
	clearCommand(command);
	return 1;
}

void clearCommand(command_t command){
	command.name[0] = 0;
}

int divideByZero(char* line){
	int x = 1;
	int y = 1;
	int z = x / --y;
	
	return 0; /* Not happening anyway */
}

int echo(char * text)
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

int startx(char * line){
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
    for (i = 0; i < 4000; i++)
		printf(" ");
        //System.device[DEVICE_SCREEN]->driver->write(DEVICE_SCREEN, " ", 1);
}

int top (char* line) {
    System.name("top");

    int i;
    task_t t;

    //System.name("printTasks");
    
    for (i = 0; i < NUM_TASKS; i++)
        if (System.tasks[i].tid != 0) {
            t = &(System.tasks[i]);
            printf("Task %d\tPriority %d\tRank %d\tUsage %d\t%s (%d)\n", t->tid, t->tpriority, t->trank, 0, t->tname, t->tstatus);

        }
}    
