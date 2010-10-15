#include "../include/shell.h"

#define SHELL_PROMPT "SuciOS_tty%d$ "

#define NUM_COMMANDS 7

static struct {
    char line[LINEBUF_LEN];
    int pos;
} linebuffer = {{0}, 0};

static struct {
    char name[LINEBUF_LEN];
    char args[LINEBUF_LEN - 2];
} command;

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
		         {"clear", "Clear the screen", clear}
};


void shell() {
    static char show_prompt = 1;
	unsigned char a;
    int exit_status,i;
    
    if (show_prompt)
        printf(SHELL_PROMPT, 1);

	switch(a = getchar()) {
	
		case '\n':
		    /* Print the newline and finalize the linebuffer string with 0: */
			putchar('\n');
		    linebuffer.line[linebuffer.pos] = 0;
		    
		    parse_command();
		    
			/* Once parsing is over, reset the linebuffer and shell status: */
		    show_prompt = 1;
			linebuffer.line[0] = linebuffer.pos = 0;
			
			exit_status = run_command();
			
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

void parse_command() {
    int initpos = 0;
    
    /* Remove preceding whitespaces: */
    while ( (linebuffer.line[initpos] == ' ') && (++initpos < LINEBUF_LEN - 1) );
    sscanf(linebuffer.line + initpos, "%s %s", command.name, command.args);
}

void input_handler(){

}

int run_command(){
	int i;
	if(streq(command.name, ""))
		return 0;
		
	for (i = 0; i < NUM_COMMANDS; i++) {
	    if (streq(command.name, commands[i].name)) {
	        clearCommand();
	        return System.exec(commands[i].function, command.args);
	    }
	}

	printf("%s: command not found\n", command.name);	
	clearCommand();
	return 1;
}

void clearCommand(){
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
        System.device[DEVICE_SCREEN]->driver->write(DEVICE_SCREEN, " ", 1);
}
