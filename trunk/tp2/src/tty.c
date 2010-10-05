#include "../include/tty.h"
void initializeInput(input_t * input);
void initializeOutput(output_t * output);

tty_t tty[NTTYS];
int currentTTY = 0;

void initialize_ttys(){
	int i = 0;
	for(i = 0; i < NTTYS; i++){
		initialize_tty(&tty[i]);
	}
}

void initialize_tty(tty_t * tty){
	initializeInput(&tty->input);
	initializeOutput(&tty->output);
}

void initializeInput(input_t * input){
	input->flags.shift_status = 0;
	input->flags.mayus_status = 0;
	input->flags.num_status = 0;
	input->flags.scroll_status = 0;
	input->flags.accent_status = 0;
	input->flags.escaped = 0;
}

void initializeOutput(output_t * output){
	output->wpos = 0;
	output->rpos = 0;
}
