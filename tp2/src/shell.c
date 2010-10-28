#include "../include/shell.h"

#define NUM_COMMANDS 9

#define SHELL_PROMPT "SuciOS_tty%d$ "

static struct {
    char line[LINEBUF_LEN];
    int pos;
} linebuffer = {{0}, 0};
/*
static struct {
    char name[LINEBUF_LEN];
    char args[LINEBUF_LEN - 2];
} command;
*/
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
		         {"top", "Shows active processes and it's CPU percentage of use", _top_run},
		         {"testTasks", "Tests tasks by creating 3, killing one and ending the other 2", testTasks}
};


void shell(int tty) {
    static char show_prompt = 1;
	unsigned char a;
    int exit_status,i;
    
    if (show_prompt)
        printf(SHELL_PROMPT, tty);

	switch(a = getchar()) {
	
		case '\n':
		    /* Print the newline and finalize the linebuffer string with 0: */
			putchar('\n');
		    ttys[System.atty].input.linebuffer.line[ttys[System.atty].input.linebuffer.pos] = 0;
		    
		    parse_command();
		    
			/* Once parsing is over, reset the linebuffer and shell status: */
		    show_prompt = 1;
			ttys[System.atty].input.linebuffer.line[0] = ttys[System.atty].input.linebuffer.pos = 0;
			
			exit_status = run_command();
			
			break;
			
		case '\b':
    		show_prompt = 0; /* Do not reprint prompt after backspace */
			if(ttys[System.atty].input.linebuffer.pos != 0) {
				ttys[System.atty].input.linebuffer.line[ttys[System.atty].input.linebuffer.pos--] = 0x00;				
				putchar('\b');
			}
			break;
			
		default:
    		show_prompt = 0; /* Do not reprint prompt after character entry */		
			if(ttys[System.atty].input.linebuffer.pos < LINEBUF_LEN - 1) {
				ttys[System.atty].input.linebuffer.line[ttys[System.atty].input.linebuffer.pos++] = a;
				putchar(a);
			}
			
			break;
	}
}

void parse_command() {
    int initpos = 0;
    
    /* Remove preceding whitespaces: */
    while ( (ttys[System.atty].input.linebuffer.line[initpos] == ' ') && (++initpos < LINEBUF_LEN - 1) );
    sscanf(ttys[System.atty].input.linebuffer.line + initpos, "%s %s", ttys[System.atty].input.command.name, ttys[System.atty].input.command.args);
}

void input_handler(){

}

int run_command(){
	int i;
	if(streq(ttys[System.atty].input.command.name, ""))
		return 0;
		
	for (i = 0; i < NUM_COMMANDS; i++) {
	    if (streq(ttys[System.atty].input.command.name, commands[i].name)) {
	        clearCommand();
	        return System.exec(commands[i].function, ttys[System.atty].input.command.args);
	    }
	}

	printf("%s: command not found\n", ttys[System.atty].input.command.name);	
	clearCommand();
	return 1;
}

void clearCommand(){
	ttys[System.atty].input.command.name[0] = 0;
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
