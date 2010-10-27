#include "../include/tty.h"
void _initializeInput(input_t * input);
void _initializeOutput(output_t * output);
void _initialize_tty(tty_t * tty);
void _load();
void _jumpToTTY(int ntty);
void _createTtys();
int _createTty(char * a);
void _switchTTY(int ntty);

/* Namespace structure */
struct TTYSNamespace TTYS = {
	_initialize_ttys,
	_refresh,
	_save,
	_load,
	_jumpToTTY,
	_createTtys,
	_switchTTY
};

void _createTtys(){
	Task.new(&(System.tasks[Task.findSlot()]), "Shell_1", _createTty, RANK_NORMAL, PRIORITY_HIGH, RUNNING_FRONT, TTY0);
	Task.new(&(System.tasks[Task.findSlot()]), "Shell_2", _createTty, RANK_NORMAL, PRIORITY_HIGH, RUNNING_FRONT, TTY1);
	Task.new(&(System.tasks[Task.findSlot()]), "Shell_3", _createTty, RANK_NORMAL, PRIORITY_HIGH, RUNNING_FRONT, TTY2);
	Task.new(&(System.tasks[Task.findSlot()]), "Shell_4", _createTty, RANK_NORMAL, PRIORITY_HIGH, RUNNING_FRONT, TTY3);
}

int _createTty(char * a){
	shellloop();
}

void _initialize_ttys(){
	int i = 0;
	for(i = 0; i < NTTYS; i++){
		_initialize_tty(&ttys[i]);
	}
	System.atty = 0;
}

void _initialize_tty(tty_t * tty){
	tty->status = TTY_READY;
	_initializeInput(&tty->input);
	_initializeOutput(&tty->output);
}

void _initializeInput(input_t * input){
	input->flags.shift_status = 0;
	input->flags.mayus_status = 0;
	input->flags.num_status = 0;
	input->flags.scroll_status = 0;
	input->flags.accent_status = 0;
	input->flags.escaped = 0;
}

void _initializeOutput(output_t * output){
	output->wpos = 0;
	output->rpos = 0;
	k_clear_screen((void *)output->address); 
}

void _refresh(){
	System.device[DEVICE_TTY]->wpos = ttys[System.atty].output.wpos;
	System.device[DEVICE_TTY]->rpos = ttys[System.atty].output.rpos;
	System.device[DEVICE_TTY]->addr = (void*) ttys[System.atty].output.address;	

	_memcpy(System.device[DEVICE_TTY]->addr, System.device[DEVICE_SCREEN]->addr, VIDEO_SIZE);

	move_cursor(System.device[DEVICE_TTY]->wpos / 2);

	System.device[DEVICE_SCREEN]->wpos = System.device[DEVICE_TTY]->wpos;
	ttys[System.atty].output.wpos = System.device[DEVICE_SCREEN]->wpos;
}

void _save(){
	ttys[System.atty].output.wpos = System.device[DEVICE_SCREEN]->wpos;
	ttys[System.atty].output.rpos = System.device[DEVICE_SCREEN]->rpos;
}

void _load(){
	System.device[DEVICE_TTY]->wpos = System.device[DEVICE_SCREEN]->wpos = ttys[System.atty].output.wpos;
	System.device[DEVICE_TTY]->rpos = System.device[DEVICE_SCREEN]->rpos = ttys[System.atty].output.rpos;
	System.device[DEVICE_TTY]->addr = (void*) ttys[System.atty].output.address;
}

void _jumpToTTY(int ntty){
	TTYS.save();
	System.atty = ntty;
	TTYS.load();
	TTYS.refresh();
	Keyboard.updateLeds();
}

void _switchTTY(int ntty){
	System.device[DEVICE_TTY]->wpos = ttys[ntty].output.wpos;
	System.device[DEVICE_TTY]->rpos = ttys[ntty].output.rpos;
	System.device[DEVICE_TTY]->addr = ttys[ntty].output.address;
}

