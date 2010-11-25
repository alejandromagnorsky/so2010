#include "../include/task.h"

static char* doNothingName1 = "doNothing1";
static char* doNothingName2 = "doNothing2";
static char* doNothingName3 = "doNothing3";
static char* doNothingName4 = "doNothing4";

int doNothing4Times(char* line) {
	System.name(doNothingName1);
	System.exec(doNothing2, "2");
    for(;;);
    return 0;
}

int doNothing2(char * line)
{
	System.name(doNothingName2);
	System.exec(doNothing3, "3");
	for(;;);
	return 0;
}

int doNothing3(char * line)
{
	System.name(doNothingName3);
	System.exec(doNothing4, "4");
	for(;;);
	return 0;
}

int doNothing4(char * line)
{
	System.name(doNothingName4);
	for(;;);
	return 0;
}

