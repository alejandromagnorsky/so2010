#include "../include/kernel.h"

DESCR_INT idt[0xFF];			/* IDT de 10 entradas*/
IDTR idtr;				/* IDTR */
unsigned char keyBuff[BUFFER_SIZE];
int tickpos = -2;

/************************************************
** Global system data structure initialization:
*************************************************/

struct device_t _sys_kbuffer = { DEVICE_KEYBOARD,        /* Device ID */
                                      "keyboard",        /* Device name */
                                      &KeyboardDriver,   /* Device driver */
                                      &keyBuff,          /* Mem starting addr */
                                      BUFFER_SIZE,       /* Mem area size */
                                      
                                      0,                 /* Current wposition */
				                      0,                 /* Current rposition */
                                    };

struct device_t _sys_screen = {	DEVICE_SCREEN,             /* Device ID */
										"screen",          /* Device name */
										&ScreenDriver,     /* Device driver */
										(void*) 0xb8000,   /* Mem starting addr */
										BUFFER_SIZE,       /* Mem area size */
										
										0,                 /* Current wposition */
										0,                 /* Current rposition */
									};


struct system_t System = {     0,               /* Tick count */
                                   2,               /* Number of devices */
                                   {&_sys_screen,
                                    &_sys_kbuffer },/* Device array */
                                   0,                /* Active terminal index */
                                   NULL,	/* Idle task */
                                   NULL,	/* Currently active task */
                                   {},		/* Tasks */
                                   1,		/* Next available tid */                               
                                   _sys_addTick,
                                   _sys_getTicks,
                                   _sys_write,
                                   _sys_read,
                                   _sys_seekr,
                                   _sys_seekw,
                                   _sys_tellr,
                                   _sys_tellw,
                                   _sys_malloc,
                                   _sys_free,
                                   _sys_exec
                               };
                               
//system_t System = &_system_data;


void _sys_addTick(system_t sys) {
    System.ticks++;
}

long int _sys_getTicks(system_t sys) {
    return System.ticks;
}


/***********************************************
** Kernel Exception routines:
************************************************/

unsigned char *exception_messages[] =
{
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void fault_handler(struct regs *r)
{
    if (r->int_no < 32)
    {
        printf("%s", exception_messages[r->int_no]);
        printf(" Exception. Goodbye!\n");
    }
    _Sti();
    shellloop();
}


/************************************************
** Kernel interrupt routines:
*************************************************/

/* Routine for IRQ0: Timer Tick. */
void int_20() {
	System.addTick();
	
}

/* Routine for IRQ1: keyboard */
void int_21(){
    unsigned char scan;
    unsigned char ascii;

    // Our interruption handler left the scan code byte in AL
    MOVFROM_AL(scan);
	
	keyEntry keyPressed = scan2ascii(scan);
	if(keyPressed->type == PRINTABLE_KEY) {
		ascii = keyPressed->ascii;
		System.write(DEVICE_KEYBOARD, &ascii, 1);
		
	} else if(keyPressed->type == SPECIAL_KEY) {
		escapedKey(keyPressed->scan_code);
	}  
     
}

void int_80() {
    /* SYSTEM CALL INTERRUPTION */
    char syscall;
    int ebx, ecx, edx, ret;
    
    /* The above instructions, as well as these macros, do not affect the
       general purpose registers, so we can read their values just fine. */
    MOVFROM_AL(syscall);
    MOVFROM_EBX(ebx);
    MOVFROM_ECX(ecx);
    MOVFROM_EDX(edx);

    switch(syscall) {
    
        case SYSTEM_CALL_WRITE:
            if (System.device[ebx]->driver->write)
                ret = System.device[ebx]->driver->write(ebx, (void*) ecx, edx);
                
            MOVTO_EAX(ret);
            break;
            
        case SYSTEM_CALL_READ:
            if (System.device[ebx]->driver->read)
                ret = System.device[ebx]->driver->read(ebx, (void*) ecx, edx);
                
            MOVTO_EAX(ret);
            break;
            
        case SYSTEM_CALL_SEEKR:
            if (System.device[ebx]->driver->seekr)
                ret = System.device[ebx]->driver->seekr(ebx, ecx, edx);
                
            MOVTO_EAX(ret);
            break;
            
        case SYSTEM_CALL_SEEKW:
            if (System.device[ebx]->driver->seekw)
                ret = System.device[ebx]->driver->seekw(ebx, ecx, edx);
                
            MOVTO_EAX(ret);
            break;
            
        case SYSTEM_CALL_TELLR:
            if (System.device[ebx]->driver->tellr)
                ret = System.device[ebx]->driver->tellr(ebx);
                
            MOVTO_EAX(ret);
            break;
            
        case SYSTEM_CALL_TELLW:
            if (System.device[ebx]->driver->tellw)
                ret = System.device[ebx]->driver->tellw(ebx);
                
            MOVTO_EAX(ret);
            break;
            
        case SYSTEM_CALL_EXEC:
            ret = ((program_t) ebx)((char*) ecx);
            MOVTO_EAX(ret);
            break;
     }

}

/*************************************************
** kmain()
** Entry point for C code:
*************************************************/

kmain() 
{

    int i,num, r;

/* Borra la pantalla. */ 
	k_clear_screen();

/* Carga IDT */
	initializeIDT();	

/* Carga de IDTR    */

	idtr.base = 0;  
	idtr.base +=(dword) &idt;
	idtr.limit = sizeof(idt)-1;
	
	_lidt (&idtr);	

	_Cli();
/* Inicializo offsets de los PICS */
	initialize_pics(0x20, 0x28);

	initializeKeyboard();
/* Habilito IRQ0 e IRQ1 (Timer Tick y Teclado) */
    _mascaraPIC1(0xFC);
    _mascaraPIC2(0xFF);

//    Paging.start();

//	Task.setupScheduler();
    _task_setupScheduler();

	_Sti();

	shellloop();

}

shellloop(){
  	while(1)
        {
		    shell();
        }
}

void initializeIDT(){
	/* set IRQ routines  */
	setup_IDT_entry (&idt[0x00], 0x08, (dword)&_int_00_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x01], 0x08, (dword)&_int_01_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x02], 0x08, (dword)&_int_02_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x03], 0x08, (dword)&_int_03_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x04], 0x08, (dword)&_int_04_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x05], 0x08, (dword)&_int_05_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x06], 0x08, (dword)&_int_06_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x07], 0x08, (dword)&_int_07_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x08], 0x08, (dword)&_int_08_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x09], 0x08, (dword)&_int_09_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x0A], 0x08, (dword)&_int_0A_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x0B], 0x08, (dword)&_int_0B_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x0C], 0x08, (dword)&_int_0C_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x0D], 0x08, (dword)&_int_0D_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x0E], 0x08, (dword)&_int_0E_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x0F], 0x08, (dword)&_int_0F_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x10], 0x08, (dword)&_int_10_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x11], 0x08, (dword)&_int_11_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x12], 0x08, (dword)&_int_12_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x13], 0x08, (dword)&_int_13_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x14], 0x08, (dword)&_int_14_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x15], 0x08, (dword)&_int_15_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x16], 0x08, (dword)&_int_16_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x17], 0x08, (dword)&_int_17_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x18], 0x08, (dword)&_int_18_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x19], 0x08, (dword)&_int_19_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x1A], 0x08, (dword)&_int_1A_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x1B], 0x08, (dword)&_int_1B_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x1C], 0x08, (dword)&_int_1C_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x1D], 0x08, (dword)&_int_1D_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x1E], 0x08, (dword)&_int_1E_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x1F], 0x08, (dword)&_int_1F_hand, ACS_INT, 0);
	
	setup_IDT_entry (&idt[0x20], 0x08, (dword)&_int_20_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x21], 0x08, (dword)&_int_21_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x80], 0x08, (dword)&_int_80_hand, ACS_INT, 0);	
}

