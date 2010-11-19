#include "../include/wini.h"

struct hdmap {
	char *name;
	task_t *task;
} hdmap[] = {
	#if ENABLE_AT_WINI
	{"at", at_winchester_task},
	#endif

	#if ENABLE_BIOS_WINI
	{"bios", bios_winchester_task},
	#endif
};

void winchester_task(){

		

}


