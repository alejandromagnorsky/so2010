#include "../include/kernel.h"

DESCR_INT idt[0xFF];			/* IDT de 10 entradas*/
IDTR idtr;				/* IDTR */
unsigned char keyBuff[BUFFER_SIZE];
int tickpos = -2;

char scheduling = 0;

tty_t ttys[NTTYS];

/************************************************
** Global system data structure initialization:
*************************************************/

struct device_t _sys_kbuffer = { DEVICE_KEYBOARD,  /* Device ID */
		                        "keyboard",        /* Device name */
		                        &KeyboardDriver,   /* Device driver */
		                        &keyBuff,          /* Mem starting addr */
		                        BUFFER_SIZE,       /* Mem area size */
		                          
		                        0,                 /* Current wposition */
					            0,                 /* Current rposition */
                            };

struct device_t _sys_screen = {	DEVICE_SCREEN,     /* Device ID */
								"screen",          /* Device name */
								&ScreenDriver,     /* Device driver */
								(void*) 0xb8000,   /* Mem starting addr */
								BUFFER_SIZE,       /* Mem area size */
						
								0,                 /* Current wposition */
								0,                 /* Current rposition */
							};

struct device_t _sys_tty = {DEVICE_TTY,
							"tty",
							&TtyDriver,
							NULL,
							VIDEO_SIZE,
							0,
							0,
							};


struct system_t System = {     0,					/* Tick count */
                               3,               	/* Number of devices */
                               {&_sys_screen,
                                &_sys_kbuffer,
								&_sys_tty},			/* Device array */

                               0,                	/* Active terminal index */

                               NULL,				/* Idle task */
                               NULL,				/* Currently active task */
                               {0},					/* Tasks */
                               {0},					/* ttys tids */
                               {0},					/* Last 100 tasks */
                               0,					/* Last 100 tasks counter */
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
                               _sys_exec,
                               _sys_gettid,
                               _sys_nexttid,
                               _sys_getrank,
                               _sys_getprio,
                               _sys_getcpuc,
                               _sys_name
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
        printf(" Exception.\n");
    }
    _Sti();
    //shellloop();
	while(1);
}

/************************************************
** Kernel interrupt routines:
*************************************************/

/* Routine for IRQ0: Timer Tick. */
void int_20() {
	System.addTick();
	//static char * vid = (char*) 0xB8000;
	//vid++;
	//vid = 'a';
	//printf("tick ");
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
		switch(keyPressed->scan_code){
			case F1:
				TTYS.jumpToTTY(TTY0);
				break;
			case F2:
				TTYS.jumpToTTY(TTY1);
				break;
			case F3:
				TTYS.jumpToTTY(TTY2);
				break;
			case F4:
				TTYS.jumpToTTY(TTY3);
				break;
			default:
				escapedKey(keyPressed->scan_code);
		}
	}
}

void int_80() {
    /* SYSTEM CALL INTERRUPTION */
    int syscall;
    task_t task;
    char* name;
    int ebx, ecx, edx, ret, iter;
    
    /* The above instructions, as well as these macros, do not affect the
       general purpose registers, so we can read their values just fine. */
    MOVFROM_EAX(syscall);
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
            ret = Task.new(&(System.tasks[Task.findSlot()]), "TaskPUTO", 
            				(program_t) ebx, RANK_NORMAL, PRIORITY_MEDIUM, RUNNING_FRONT, System.task->tty, (char*) ecx);

            MOVTO_EAX(ret);
            
            break;

        case SYSTEM_CALL_GETTID:
            ret = System.task->tid;
            
            MOVTO_EAX(ret)
            break;

        case SYSTEM_CALL_NEXTTID:
            iter = (* (int*) ebx) % NUM_TASKS;
            
            while (System.tasks[iter = (iter + 1) % NUM_TASKS].tid == 0);
            
            ret = System.tasks[iter].tid;
            
            * (int*) ebx = iter;
            
            MOVTO_EAX(ret);
            break;

        case SYSTEM_CALL_GETRANK:
            if (ebx == 0)
                ebx = System.task->tid;
                
            ret = (task = Task.getByTID(ebx)) ? task->trank : -1;
            MOVTO_EAX(ret);
            break;

        case SYSTEM_CALL_GETPRIO:
            if (ebx == 0)
                ebx = System.task->tid;
            ret = (task = Task.getByTID(ebx)) ? task->tpriority : -1;
            
            MOVTO_EAX(ret);
            break;

        case SYSTEM_CALL_GETCPUC:
            if (ebx == 0)
                ebx = System.task->tid;
            ret = (task = Task.getByTID(ebx)) ? _top_processCpuUsage(ebx) : -1;
            
            MOVTO_EAX(ret);
            break;

        case SYSTEM_CALL_NAME:
            name = (char*) ebx;
            
            if (name != NULL) {
                strncpy(name, System.task->tname, MAX_TASK_NAME);
                name[MAX_TASK_NAME] = 0;
            }
            
            MOVTO_EAX(System.task->tname);
            break;

     }

}

int printTasks(char* line) {
    int i;
    task_t t;

    //System.name("printTasks");
    
    for (i = 0; i < NUM_TASKS; i++)
        if (System.tasks[i].tid != 0) {
            t = &(System.tasks[i]);
            printf("Task %d\tPriority %d\tRank %d\tUsage %d\t%s (%d)\n", t->tid, t->tpriority, t->trank, 0, t->tname, t->tstatus);

        }

    for(;;);
    /*task_t hola;
    int iter = 30, self, first, other;
    int rank, prio, usage;
    
    self = System.gettid();

    iter = 0;
    other = first = System.nexttid(&iter);
    
    do {
        rank = System.getrank(other);
        prio = System.getprio(other);
        usage = System.getcpuc(other);

        hola = Task.getByTID(other);
        
        printf("Task %d\tPriority %d\tRank %d\tUsage %d\tName %s\n", other, rank, prio, usage, hola->tname);
        other = System.nexttid(&iter);
    } while (first != other);*/
}

/*************************************************
** kmain()
** Entry point for C code:
*************************************************/

kmain(multiboot_info_t* mbd, unsigned int magic) 
{
    int i,num, r;
    _Cli();
    
    /* Borra la pantalla. */ 
    k_clear_screen((void*) 0xb8000);

    /* Dame paz */
    _mascaraPIC1(0xFF);
    _mascaraPIC2(0xFF);
    
    /* Carga IDT */
    initializeIDT();        

    /* Carga de IDTR    */

    idtr.base = 0;  
    idtr.base +=(dword) &idt;
    idtr.limit = sizeof(idt)-1;

    _lidt (&idtr);  

    printf("________________________________________________________________________________\n");
    printf("Welcome to SuciOS\n");
    printf("________________________________________________________________________________\n");

    /* Inicializo offsets de los PICS */
    printf("Initializing PICs ............. ");
    initialize_pics(0x20, 0x28);
    printf("OK\n\n");

    printf("Initializing Keyboard ......... ");
    initializeKeyboard();
    printf("OK\n\n");


    printf("Memory detected: %d KB\n", (mbd->mem_lower + mbd->mem_upper));
    printf("Paging the memory ............. ");
    Paging.start(mbd->mem_lower + mbd->mem_upper);
    printf("OK\n\n");

	Task.setupScheduler();

    TTYS.initialize(TTY0);
    TTYS.refresh();
    
	System.atty = TTY0;
	TTYS.runShells();
    
	//testTasks('a');
	
    /* Gracias */
    _mascaraPIC1(0xFC);
    _mascaraPIC2(0xFF);

	_Sti();
    
    shellloop();
}


#define SHELL_PROMPT "SuciOS_tty%d$ "

shellloop(){
	
	linebuffer_t linebuffer = {{0},0};
	command_t command;
	int tty_number = Task.getTty(System.task);

	char show_prompt = 1;
	unsigned char a;
    int exit_status,i;

	while(1){
		if(show_prompt)
			printf(SHELL_PROMPT, tty_number);

		switch(a = getchar()){
			case '\n':
				putchar('\n');
				linebuffer.line[linebuffer.pos] = 0;
				parse_command(linebuffer, command);
				show_prompt = 1;
				linebuffer.line[0] = linebuffer.pos = 0;
				exit_status = run_command(command);
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
