int gralprotection(char * input){
	   asm("mov 0, %eax");
	   asm("push %eax");
	   asm("iret");
	   return 0; /* Will never get here anyway ^^ */
 }