#include "../include/tty.h"
void _initializeInput(input_t * input);
void _initializeOutput(output_t * output);
void _initialize_tty(tty_t * tty);
void _load();
void _jumpToTTY(int ntty);
void _runShells();
int _createTty(char * a);
void _setTTY(int ntty);
void _setKeyboard(int ntty);
void _saveTTY(int ntty);
void _saveKeyboard(int ntty);


/* Namespace structure */
struct TTYSNamespace TTYS = {
	_initialize_ttys,
	_refresh,
	_save,
	_load,
	_jumpToTTY,
	_runShells,
	_setTTY,
	_setKeyboard,
	_saveTTY,
	_saveKeyboard
};

void _runShells(){
	task_t task;
	task = &(System.tasks[Task.findSlot()]);
	Task.new(&(System.tasks[Task.findSlot()]), "Shell_1", _createTty, RANK_NORMAL, PRIORITY_HIGH, RUNNING_FRONT, TTY0, NULL);
	System.ttysTids[0] = task->tid;
	
	task = &(System.tasks[Task.findSlot()]);
	Task.new(&(System.tasks[Task.findSlot()]), "Shell_2", _createTty, RANK_NORMAL, PRIORITY_HIGH, RUNNING_FRONT, TTY1, NULL);
	System.ttysTids[1] = task->tid;
	
	task = &(System.tasks[Task.findSlot()]);
	Task.new(&(System.tasks[Task.findSlot()]), "Shell_3", _createTty, RANK_NORMAL, PRIORITY_HIGH, RUNNING_FRONT, TTY2, NULL);
	System.ttysTids[2] = task->tid;
	
	task = &(System.tasks[Task.findSlot()]);
	Task.new(&(System.tasks[Task.findSlot()]), "Shell_4", _createTty, RANK_NORMAL, PRIORITY_HIGH, RUNNING_FRONT, TTY3, NULL);
	System.ttysTids[3] = task->tid;
}

int _createTty(char * a){
	shell();
}

void _initialize_ttys(int def_tty){
	int i = 0;
	for(i = 0; i < NTTYS; i++){
		_initialize_tty(&ttys[i]);
	}
	System.atty = def_tty;
	TTYS.setTTY(System.atty);
	TTYS.setKeyboard(System.atty);
	TTYS.refresh();
}

void _initialize_tty(tty_t * tty){
	tty->status = TTY_READY;
	_initializeInput(&tty->input);
	_initializeOutput(&tty->output);
}

void _initializeInput(input_t * input){
	input->inputbuffer.wpos = 0;
	input->inputbuffer.rpos = 0;
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
	_memcpy(ttys[System.atty].output.address, System.device[DEVICE_SCREEN]->addr, VIDEO_SIZE);
	move_cursor(ttys[System.atty].output.wpos / 2);
}

void _save(){
	TTYS.saveTTY(System.atty);
	TTYS.saveKeyboard(System.atty);
}

void _load(){
	TTYS.setTTY(System.atty);
	TTYS.setKeyboard(System.atty);
}

void _jumpToTTY(int ntty){
	if(ntty != System.atty){
		TTYS.save();
		System.atty = ntty;
		TTYS.load();
		TTYS.refresh();
	}
}

void _setTTY(int ntty){
	System.device[DEVICE_TTY]->wpos = ttys[ntty].output.wpos;
	System.device[DEVICE_TTY]->rpos = ttys[ntty].output.rpos;
	System.device[DEVICE_TTY]->addr = ttys[ntty].output.address;
}

void _setKeyboard(int ntty){
	System.device[DEVICE_KEYBOARD]->wpos = ttys[ntty].input.inputbuffer.wpos;
	System.device[DEVICE_KEYBOARD]->rpos = ttys[ntty].input.inputbuffer.rpos;
	System.device[DEVICE_KEYBOARD]->addr = ttys[ntty].input.inputbuffer.address;
}

void _saveTTY(int ntty){
	ttys[ntty].output.wpos = System.device[DEVICE_TTY]->wpos;
	ttys[ntty].output.rpos = System.device[DEVICE_TTY]->rpos;
}

void _saveKeyboard(int ntty){
	ttys[ntty].input.inputbuffer.wpos = System.device[DEVICE_TTY]->wpos;
	ttys[ntty].input.inputbuffer.rpos = System.device[DEVICE_TTY]->rpos;
}
