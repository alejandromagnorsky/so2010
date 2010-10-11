#include "../include/tty.h"
void _initializeInput(input_t * input);
void _initializeOutput(output_t * output);
void _initialize_tty(tty_t * tty);

/* Namespace structure */
struct TTYSNamespace TTYS = {
	_initialize_ttys,
	_refresh,
	_update
};


void _initialize_ttys(){
	int i = 0;
	for(i = 0; i < NTTYS; i++){
		_initialize_tty(&ttys[i]);
	}
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

	System.device[DEVICE_SCREEN]->wpos = System.device[DEVICE_TTY]->wpos;
	ttys[System.atty].output.wpos = System.device[DEVICE_SCREEN]->wpos;
}

void _update(){
	if(ttys[System.atty].status == TTY_WRITTEN){
		
	}
}
