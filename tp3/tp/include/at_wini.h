#ifndef _AT_WINI_H_
#define _AT_WINI_H

/* ASSEMBLER */


void		phys_copy(	long source, long destination, long bytecount);
void		port_out(unsigned short port, unsigned int value);
void		port_in(unsigned short port, unsigned int * value);
void		_lock();
void 		_unlock();

/* PARCHES */
#define lock		_lock
#define	unlock		_unlock

#define portw_in 	_portw_in
#define portw_out 	_portw_out
#define copy_param 	copy_params
#define init_param	init_params

#define panic(S,N) printf(S);while(1)


/* </include/const.h> */
/* Copyright (C) 1987 by Prentice-Hall, Inc.  Permission is hereby granted to
 * private individuals and educational institutions to modify and
 * redistribute the binary and source programs of this system to other
 * private individuals and educational institutions for educational and
 * research purposes.  For corporate or commercial use, permission from
 * Prentice-Hall is required.  In general, such permission will be granted,
 * subject to a few conditions.
 */

#define EXTERN        extern	/* used in *.h files */
#define PRIVATE       static	/* PRIVATE x limits the scope of x */
#define PUBLIC			/* PUBLIC is the opposite of PRIVATE */
#define FORWARD 		/* some compilers require this to be 'static' */

#define TRUE               1	/* used for turning integers into Booleans */
#define FALSE              0	/* used for turning integers into Booleans */

#define HZ	          60	/* clock freq (software settable on IBM-PC) */
#define BLOCK_SIZE      1024	/* # bytes in a disk block */
#define SUPER_USER   (uid) 0	/* uid of superuser */

#define MAJOR	           8	/* major device = (dev>>MAJOR) & 0377 */
#define MINOR	           0	/* minor device = (dev>>MINOR) & 0377 */

#define NR_TASKS           8	/* number of tasks in the transfer vector */
#define NR_PROCS          16	/* number of slots in proc table */
#define NR_SEGS            3	/* # segments per process */
#define T                  0	/* proc[i].mem_map[T] is for text */
#define D                  1	/* proc[i].mem_map[D] is for data */
#define S                  2	/* proc[i].mem_map[S] is for stack */

#define MAX_P_LONG  2147483647	/* maximum positive long, i.e. 2**31 - 1 */

/* Memory is allocated in clicks. */
#define CLICK_SIZE      0020	/* unit in which memory is allocated */
#define CLICK_SHIFT        4	/* log2 of CLICK_SIZE */

/* Process numbers of some important processes */
#define MM_PROC_NR         0	/* process number of memory manager */
#define FS_PROC_NR         1	/* process number of file system */
#define INIT_PROC_NR       2	/* init -- the process that goes multiuser */
#define LOW_USER           2	/* first user not part of operating system */

/* Miscellaneous */
#define BYTE            0377	/* mask for 8 bits */
#define TO_USER            0	/* flag telling to copy from fs to user */
#define FROM_USER          1	/* flag telling to copy from user to fs */
#define READING            0	/* copy data to user */
#define WRITING            1	/* copy data from user */
#define ABS             -999	/* this process means absolute memory */

#define WORD_SIZE          2		/* number of bytes per word */

#define NIL_PTR   (char *) 0	/* generally useful expression */

#define NO_NUM           0x8000	/* used as numerical argument to panic() */
#define MAX_PATH            128	/* max length of path names */
#define SIG_PUSH_BYTES	      8	/* how many bytes pushed by signal */
#define MAX_ISTACK_BYTES   1024	/* maximum initial stack size for EXEC */

/* Device numbers of root (RAM) and boot (fd0) devices. */
#define ROOT_DEV (dev_nr)   256	/* major-minor device number of root dev */
#define BOOT_DEV (dev_nr)   512	/* major-minor device number of boot diskette */

/* Flag bits for i_mode in the inode. */
#define I_TYPE          0170000	/* this field gives inode type */
#define I_REGULAR       0100000	/* regular file, not dir or special */
#define I_BLOCK_SPECIAL 0060000	/* block special file */
#define I_DIRECTORY     0040000	/* file is a directory */
#define I_CHAR_SPECIAL  0020000	/* character special file */
#define I_SET_UID_BIT   0004000	/* set effective uid on exec */
#define I_SET_GID_BIT   0002000	/* set effective gid on exec */
#define ALL_MODES       0006777	/* all bits for user, group and others */
#define RWX_MODES       0000777	/* mode bits for RWX only */
#define R_BIT           0000004	/* Rwx protection bit */
#define W_BIT           0000002	/* rWx protection bit */
#define X_BIT           0000001	/* rwX protection bit */
#define I_NOT_ALLOC     0000000	/* this inode is free */
/* <include/const.h> */


/* </include/type.h> */
/* Macros */
#define MAX(a,b)	(a > b ? a : b)
#define MIN(a,b)	(a < b ? a : b)

/* Type definitions */
typedef unsigned short unshort;	/* must be 16-bit unsigned */
typedef unshort block_nr;	/* block number */
#define NO_BLOCK (block_nr) 0	/* indicates the absence of a block number */
#define MAX_BLOCK_NR (block_nr) 0177777

typedef unshort inode_nr;	/* inode number */
#define NO_ENTRY (inode_nr) 0	/* indicates the absence of a dir entry */
#define MAX_INODE_NR (inode_nr) 0177777

typedef unshort zone_nr;	/* zone number */
#define NO_ZONE   (zone_nr) 0	/* indicates the absence of a zone number */
#define HIGHEST_ZONE (zone_nr)  0177777

typedef unshort bit_nr;		/* if inode_nr & zone_nr both unshort,
				   then also unshort, else long */

typedef long zone_type;		/* zone size */
typedef unshort mask_bits;	/* mode bits */
typedef unshort dev_nr;		/* major | minor device number */
#define NO_DEV    (dev_nr) ~0	/* indicates absence of a device number */

typedef char links;		/* number of links to an inode */
#define MAX_LINKS 	0177

typedef long real_time;		/* real time in seconds since Jan 1, 1980 */
typedef long file_pos;		/* position in, or length of, a file */
#define MAX_FILE_POS 017777777777L
typedef short int uid;		/* user id */
typedef char gid;		/* group id */

typedef unsigned vir_bytes;	/* virtual addresses and lengths in bytes */
typedef unsigned vir_clicks;	/* virtual addresses and lengths in clicks */
typedef long phys_bytes;	/* physical addresses and lengths in bytes */
typedef unsigned phys_clicks;	/* physical addresses and lengths in clicks */
typedef int signed_clicks;	/* same length as phys_clicks, but signed */

/* Types relating to messages. */
#define M1                 1
#define M3                 3
#define M4                 4
#define M3_STRING         14

typedef struct {int m1i1, m1i2, m1i3; char *m1p1, *m1p2, *m1p3;} mess_1;
typedef struct {int m2i1, m2i2, m2i3; long m2l1, m2l2; char *m2p1;} mess_2;
typedef struct {int m3i1, m3i2; char *m3p1; char m3ca1[M3_STRING];} mess_3;
typedef struct {long m4l1, m4l2, m4l3, m4l4;} mess_4;
typedef struct {char m5c1, m5c2; int m5i1, m5i2; long m5l1, m5l2, m5l3;} mess_5;
typedef struct {int m6i1, m6i2, m6i3; long m6l1; int (*m6f1)();} mess_6;

typedef struct {
  int m_source;			/* who sent the message */
  int m_type;			/* what kind of message is it */
  union {
	mess_1 m_m1;
	mess_2 m_m2;
	mess_3 m_m3;
	mess_4 m_m4;
	mess_5 m_m5;
	mess_6 m_m6;
  } m_u;
} message;

#define MESS_SIZE (sizeof(message))
#define NIL_MESS (message *) 0

/* The following defines provide names for useful members. */
#define m1_i1  m_u.m_m1.m1i1
#define m1_i2  m_u.m_m1.m1i2
#define m1_i3  m_u.m_m1.m1i3
#define m1_p1  m_u.m_m1.m1p1
#define m1_p2  m_u.m_m1.m1p2
#define m1_p3  m_u.m_m1.m1p3

#define m2_i1  m_u.m_m2.m2i1
#define m2_i2  m_u.m_m2.m2i2
#define m2_i3  m_u.m_m2.m2i3
#define m2_l1  m_u.m_m2.m2l1
#define m2_l2  m_u.m_m2.m2l2
#define m2_p1  m_u.m_m2.m2p1

#define m3_i1  m_u.m_m3.m3i1
#define m3_i2  m_u.m_m3.m3i2
#define m3_p1  m_u.m_m3.m3p1
#define m3_ca1 m_u.m_m3.m3ca1


#define m4_l1  m_u.m_m4.m4l1
#define m4_l2  m_u.m_m4.m4l2
#define m4_l3  m_u.m_m4.m4l3
#define m4_l4  m_u.m_m4.m4l4

#define m5_c1  m_u.m_m5.m5c1
#define m5_c2  m_u.m_m5.m5c2
#define m5_i1  m_u.m_m5.m5i1
#define m5_i2  m_u.m_m5.m5i2
#define m5_l1  m_u.m_m5.m5l1
#define m5_l2  m_u.m_m5.m5l2
#define m5_l3  m_u.m_m5.m5l3

#define m6_i1  m_u.m_m6.m6i1
#define m6_i2  m_u.m_m6.m6i2
#define m6_i3  m_u.m_m6.m6i3
#define m6_l1  m_u.m_m6.m6l1
#define m6_f1  m_u.m_m6.m6f1

struct mem_map {
  vir_clicks mem_vir;		/* virtual address */
  phys_clicks mem_phys;		/* physical address */
  vir_clicks mem_len;		/* length */
};

struct copy_info {		/* used by sys_copy(src, dst, bytes) */
	int cp_src_proc;
	int cp_src_space;
	vir_bytes cp_src_vir;
	int cp_dst_proc;
	int cp_dst_space;
	vir_bytes cp_dst_vir;
	vir_bytes cp_bytes;
};
/* <include/type.h> */


/* </include/error.h> */
/* Error codes.  They are negative since a few system calls, such as READ, can
 * either return a positive number indicating success, or an error code.
 */

#define NERROR            34
#define OK                 0
#define ERROR             -1
#define EPERM             -1
#define ENOENT            -2
#define ESRCH             -3
#define EINTR             -4
#define EIO               -5
#define ENXIO             -6
#define E2BIG             -7
#define ENOEXEC           -8
#define EBADF             -9
#define ECHILD           -10
#define EAGAIN           -11
#define ENOMEM           -12
#define EACCES           -13
#define EFAULT           -14
#define ENOTBLK          -15
#define EBUSY            -16
#define EEXIST           -17
#define EXDEV            -18
#define ENODEV           -19
#define ENOTDIR          -20
#define EISDIR           -21
#define EINVAL           -22
#define ENFILE           -23
#define EMFILE           -24
#define ENOTTY           -25
#define ETXTBSY          -26
#define EFBIG            -27
#define ENOSPC           -28
#define ESPIPE           -29
#define EROFS            -30
#define EMLINK           -31
#define EPIPE            -32
#define EDOM             -33
#define ERANGE           -34

#define E_LOCKED        -101
#define E_BAD_CALL      -102
#define E_LONG_STRING   -103
#define EOF             -104	/* End Of File - used by drivers */

/* The following error codes are generated by the kernel itself. */
#define E_BAD_DEST        -1	/* destination address illegal */
#define E_BAD_SRC         -2	/* source address illegal */
#define E_TRY_AGAIN       -3	/* can't send-- tables full */
#define E_OVERRUN         -4	/* interrupt for task that is not waiting */
#define E_BAD_BUF         -5	/* message buf outside caller's addr space */
#define E_TASK            -6	/* can't send to task */
#define E_NO_MESSAGE      -7	/* RECEIVE failed: no message present */
#define E_NO_PERM         -8	/* ordinary users can't send to tasks */
#define E_BAD_FCN         -9	/* only valid fcns are SEND, RECEIVE, BOTH */
#define E_BAD_ADDR       -10	/* bad address given to utility routine */
#define E_BAD_PROC       -11	/* bad proc number given to utility */
/* <include/error.h> */



/* <include/const.h> */
/* General constants used by the kernel. */

//#ifdef i8088
/* p_reg contains: ax, bx, cx, dx, si, di, bp, es, ds, cs, ss in that order. */
#define NR_REGS           11	/* number of general regs in each proc slot */
#define INIT_PSW      0x0200	/* initial psw */
#define INIT_SP (int*)0x0010	/* initial sp: 3 words pushed by kernel */

/* The following values are used in the assembly code.  Do not change the
 * values of 'ES_REG', 'DS_REG', 'CS_REG', or 'SS_REG' without making the 
 * corresponding changes in the assembly code.
 */
#define ES_REG             7	/* proc[i].p_reg[ESREG] is saved es */
#define DS_REG             8	/* proc[i].p_reg[DSREG] is saved ds */
#define CS_REG             9	/* proc[i].p_reg[CSREG] is saved cs */
#define SS_REG            10	/* proc[i].p_reg[SSREG] is saved ss */

#define VECTOR_BYTES     284	/* bytes of interrupt vectors to save */
#define MEM_BYTES    655360L	/* memory size for /dev/mem */

/* Interrupt vectors */
#define DIVIDE_VECTOR      0	/* divide interrupt vector */
#define CLOCK_VECTOR       8	/* clock interrupt vector */
#define KEYBOARD_VECTOR    9	/* keyboard interrupt vector */
#define XT_WINI_VECTOR	  13	/* xt winchester interrupt vector */
#define FLOPPY_VECTOR     14	/* floppy disk interrupt vector */
#define PRINTER_VECTOR    15	/* line printer interrupt vector */
#define SYS_VECTOR        32	/* system calls are made with int SYSVEC */
#define AT_WINI_VECTOR	 118	/* at winchester interrupt vector */

/* The 8259A interrupt controller has to be re-enabled after each interrupt. */
#define INT_CTL         0x20	/* I/O port for interrupt controller */
#define INT_CTLMASK     0x21	/* setting bits in this port disables ints */
#define INT2_CTL	0xA0	/* I/O port for second interrupt controller */
#define INT2_MASK	0xA1	/* setting bits in this port disables ints */
#define ENABLE          0x20	/* code used to re-enable after an interrupt */
//#endif

#define TASK_STACK_BYTES 256	/* how many bytes for each task stack */
#define K_STACK_BYTES    256	/* how many bytes for the kernel stack */

#define RET_REG            0	/* system call return codes go in this reg */
#define IDLE            -999	/* 'cur_proc' = IDLE means nobody is running */

/* The following items pertain to the 3 scheduling queues. */
#define NQ                 3	/* # of scheduling queues */
#define TASK_Q             0	/* ready tasks are scheduled via queue 0 */
#define SERVER_Q           1	/* ready servers are scheduled via queue 1 */
#define USER_Q             2	/* ready users are scheduled via queue 2 */


/* </include/const.h> */



/* <include/proc.h> */
EXTERN struct proc {
  int p_reg[NR_REGS];		/* process' registers */
  int *p_sp;			/* stack pointer */
  //struct pc_psw p_pcpsw;		/* pc and psw as pushed by interrupt */
  int p_flags;			/* P_SLOT_FREE, SENDING, RECEIVING, etc. */
  struct mem_map p_map[NR_SEGS];/* memory map */
  int *p_splimit;		/* lowest legal stack value */
  int p_pid;			/* process id passed in from MM */

  real_time user_time;		/* user time in ticks */
  real_time sys_time;		/* sys time in ticks */
  real_time child_utime;	/* cumulative user time of children */
  real_time child_stime;	/* cumulative sys time of children */
  real_time p_alarm;		/* time of next alarm in ticks, or 0 */

  struct proc *p_callerq;	/* head of list of procs wishing to send */
  struct proc *p_sendlink;	/* link to next proc wishing to send */
  message *p_messbuf;		/* pointer to message buffer */
  int p_getfrom;		/* from whom does process want to receive? */

  struct proc *p_nextready;	/* pointer to next ready process */
  int p_pending;		/* bit map for pending signals 1-16 */
} proc[NR_TASKS+NR_PROCS];

/* Bits for p_flags in proc[].  A process is runnable iff p_flags == 0 */
#define P_SLOT_FREE      001	/* set when slot is not in use */
#define NO_MAP           002	/* keeps unmapped forked child from running */
#define SENDING          004	/* set when process blocked trying to send */
#define RECEIVING        010	/* set when process blocked trying to recv */

#define proc_addr(n) &proc[NR_TASKS + n]
#define NIL_PROC (struct proc *) 0

EXTERN struct proc *proc_ptr;	/* &proc[cur_proc] */
EXTERN struct proc *bill_ptr;	/* ptr to process to bill for clock ticks */
EXTERN struct proc *rdy_head[NQ];	/* pointers to ready list headers */
EXTERN struct proc *rdy_tail[NQ];	/* pointers to ready list tails */

EXTERN unsigned busy_map;		/* bit map of busy tasks */
EXTERN message *task_mess[NR_TASKS+1];	/* ptrs to messages for busy tasks */
/* </include/proc.h> */







/* </include/callnr.h> */
#define NCALLS            69  	/* number of system calls allowed */

#define EXIT	           1 
#define FORK	           2 
#define READ	           3 
#define WRITE	           4 
#define OPEN	           5 
#define CLOSE	           6 
#define WAIT	           7
#define CREAT	           8 
#define LINK	           9 
#define UNLINK	          10 
#define CHDIR	          12 
#define TIME	          13
#define MKNOD	          14 
#define CHMOD	          15 
#define CHOWN	          16 
#define BRK	          17
#define STAT	          18 
#define LSEEK	          19
#define GETPID	          20
#define MOUNT	          21 
#define UMOUNT	          22 
#define SETUID	          23
#define GETUID	          24
#define STIME	          25
#define ALARM	          27
#define FSTAT	          28 
#define PAUSE	          29
#define UTIME	          30 
#define ACCESS	          33 
#define SYNC	          36 
#define KILL	          37
#define DUP	          41 
#define PIPE	          42 
#define TIMES	          43
#define SETGID	          46
#define GETGID	          47
#define SIGNAL	          48
#define IOCTL             54
#define EXEC	          59
#define UMASK	          60 
#define CHROOT	          61 

/* The following are not system calls, but are processed like them. */
#define KSIG	          64	/* kernel detected a signal */
#define UNPAUSE           65	/* to MM or FS: check for EINTR */
#define BRK2	          66	/* to MM: used to say how big FS & INIT are */
#define REVIVE            67	/* to FS: revive a sleeping process */
#define TASK_REPLY        68	/* to FS: reply code from tty task */
/* <include/callnr.h> */






/* </include/com.h> */
/* System calls. */
#define SEND		   1	/* function code for sending messages */
#define RECEIVE		   2	/* function code for receiving messages */
#define BOTH		   3	/* function code for SEND + RECEIVE */
#define ANY   (NR_PROCS+100)	/* receive(ANY, buf) accepts from any source */

/* Task numbers, function codes and reply codes. */
#define HARDWARE          -1	/* used as source on interrupt generated msgs */

#define SYSTASK           -2	/* internal functions */
#	define SYS_XIT     1	/* fcn code for sys_xit(parent, proc) */
#	define SYS_GETSP   2	/* fcn code for sys_sp(proc, &new_sp) */
#	define SYS_SIG     3	/* fcn code for sys_sig(proc, sig) */
#	define SYS_FORK    4	/* fcn code for sys_fork(parent, child) */
#	define SYS_NEWMAP  5	/* fcn code for sys_newmap(procno, map_ptr) */
#	define SYS_COPY    6	/* fcn code for sys_copy(ptr) */
#	define SYS_EXEC    7	/* fcn code for sys_exec(procno, new_sp) */
#	define SYS_TIMES   8	/* fcn code for sys_times(procno, bufptr) */
#	define SYS_ABORT   9	/* fcn code for sys_abort() */

#define CLOCK             -3	/* clock class */
#	define SET_ALARM   1	/* fcn code to CLOCK, set up alarm */
#	define CLOCK_TICK  2	/* fcn code for clock tick */
#	define GET_TIME	   3	/* fcn code to CLOCK, get real time */
#	define SET_TIME	   4	/* fcn code to CLOCK, set real time */
#	define REAL_TIME   1	/* reply from CLOCK: here is real time */

#define MEM               -4	/* /dev/ram, /dev/(k)mem and /dev/null class */
#	define RAM_DEV     0	/* minor device for /dev/ram */
#	define MEM_DEV     1	/* minor device for /dev/mem */
#	define KMEM_DEV    2	/* minor device for /dev/kmem */
#	define NULL_DEV    3	/* minor device for /dev/null */

#define FLOPPY            -5	/* floppy disk class */
#define WINCHESTER        -6	/* winchester (hard) disk class */
#	define DISKINT     1	/* fcn code for disk interupt */
#	define DISK_READ   3	/* fcn code to DISK (must equal TTY_READ) */
#	define DISK_WRITE  4	/* fcn code to DISK (must equal TTY_WRITE) */
#	define DISK_IOCTL  5	/* fcn code for setting up RAM disk */

#define TTY               -7	/* terminal I/O class */
#define PRINTER           -8	/* printer  I/O class */
#	define TTY_CHAR_INT 1	/* fcn code for tty input interrupt */
#	define TTY_O_DONE  2	/* fcn code for tty output done */
#	define TTY_READ	   3	/* fcn code for reading from tty */
#	define TTY_WRITE   4	/* fcn code for writing to tty */
#	define TTY_IOCTL   5	/* fcn code for ioctl */
#	define SUSPEND	-998	/* used in interrupts when tty has no data */

/* Names of message fields for messages to CLOCK task. */
#define DELTA_TICKS    m6_l1	/* alarm interval in clock ticks */
#define FUNC_TO_CALL   m6_f1	/* pointer to function to call */
#define NEW_TIME       m6_l1	/* value to set clock to (SET_TIME) */
#define CLOCK_PROC_NR  m6_i1	/* which proc (or task) wants the alarm? */
#define SECONDS_LEFT   m6_l1	/* how many seconds were remaining */

/* Names of message fields used for messages to block and character tasks. */
#define DEVICE         m2_i1	/* major-minor device */
#define PROC_NR        m2_i2	/* which (proc) wants I/O? */
#define COUNT          m2_i3	/* how many bytes to transfer */
#define POSITION       m2_l1	/* file offset */
#define ADDRESS        m2_p1	/* core buffer address */

/* Names of message fields for messages to TTY task. */
#define TTY_LINE       m2_i1	/* message parameter: terminal line */
#define TTY_REQUEST    m2_i3	/* message parameter: ioctl request code */
#define TTY_SPEK       m2_l1	/* message parameter: ioctl speed, erasing */
#define TTY_FLAGS      m2_l2	/* message parameter: ioctl tty mode */

/* Names of messages fields used in reply messages from tasks. */
#define REP_PROC_NR    m2_i1	/* # of proc on whose behalf I/O was done */
#define REP_STATUS     m2_i2	/* bytes transferred or error number */

/* Names of fields for copy message to SYSTASK. */
#define SRC_SPACE      m5_c1	/* T or D space (stack is also D) */
#define SRC_PROC_NR    m5_i1	/* process to copy from */
#define SRC_BUFFER     m5_l1	/* virtual address where data come from */
#define DST_SPACE      m5_c2	/* T or D space (stack is also D) */
#define DST_PROC_NR    m5_i2	/* process to copy to */
#define DST_BUFFER     m5_l2	/* virtual address where data go to */
#define COPY_BYTES     m5_l3	/* number of bytes to copy */

/* Field names for accounting, SYSTASK and miscellaneous. */
#define USER_TIME      m4_l1	/* user time consumed by process */
#define SYSTEM_TIME    m4_l2	/* system time consumed by process */
#define CHILD_UTIME    m4_l3	/* user time consumed by process' children */
#define CHILD_STIME    m4_l4	/* system time consumed by proces children */

#define PROC1          m1_i1	/* indicates a process */
#define PROC2          m1_i2	/* indicates a process */
#define PID            m1_i3	/* process id passed from MM to kernel */
#define STACK_PTR      m1_p1	/* used for stack ptr in sys_exec, sys_getsp */
#define PR             m6_i1	/* process number for sys_sig */
#define SIGNUM         m6_i2	/* signal number for sys_sig */
#define FUNC           m6_f1	/* function pointer for sys_sig */
#define MEM_PTR        m1_p1	/* tells where memory map is for sys_newmap */
#define CANCEL             0    /* general request to force a task to cancel */
#define SIG_MAP        m1_i2	/* used by kernel for passing signal bit map */
/* <include/com.h> */





/* </include/sgtty.h> */
/* Data structures for IOCTL. */

struct sgttyb {
  char sg_ispeed;		/* input speed (not used) */
  char sg_ospeed;		/* output speed (not used) */
  char sg_erase;		/* erase character */
  char sg_kill;			/* kill character */
  int  sg_flags;		/* mode flags */
};

struct tchars {
  char t_intrc;			/* SIGINT char */
  char t_quitc;			/* SIGQUIT char */
  char t_startc;		/* start output (initially CTRL-Q) */
  char t_stopc;			/* stop output	(initially CTRL-S) */
  char t_eofc;			/* EOF (initially CTRL-D) */
  char t_brkc;			/* input delimiter (like nl) */
};

/* Fields in t_flags. */
#define XTABS	     0006000	/* do tab expansion */
#define RAW	     0000040	/* enable raw mode */
#define CRMOD	     0000020	/* map lf to cr + lf */
#define ECHO	     0000010	/* echo input */
#define CBREAK	     0000002	/* enable cbreak mode */
#define COOKED       0000000	/* neither CBREAK nor RAW */

#define TIOCGETP (('t'<<8) | 8)
#define TIOCSETP (('t'<<8) | 9)
#define TIOCGETC (('t'<<8) | 18)
#define TIOCSETC (('t'<<8) | 17)
/* <include/sgtty.h> */







/* </include/signal.h> */
#define NR_SIGS           16	/* number of signals used */
#define NSIG              16	/* number of signals used */

#define	SIGHUP	           1	/* hangup */
#define SIGINT             2	/* interrupt (DEL) */
#define SIGQUIT            3	/* quit (ASCII FS) */
#define SIGILL             4	/* illegal instruction (not reset when caught)*/
#define SIGTRAP            5	/* trace trap (not reset when caught) */
#define SIGIOT             6	/* IOT instruction */
#define SIGEMT             7	/* EMT instruction */
#define SIGFPE             8	/* floating point exception */
#define SIGKILL            9	/* kill (cannot be caught or ignored) */
#define SIGBUS            10	/* bus error */
#define SIGSEGV           11	/* segmentation violation */
#define SIGSYS            12	/* bad argument to system call */
#define SIGPIPE           13	/* write on a pipe with no one to read it */
#define SIGALRM           14	/* alarm clock */
#define SIGTERM           15	/* software termination signal from kill */

#define STACK_FAULT       16	/* used by kernel to signal stack fault */

int	(*signal())();
#define	SIG_DFL	(int (*)())0
#define	SIG_IGN	(int (*)())1
/* <include/signal.h> */







/* </include/stat.h> */
struct stat {
    short int st_dev;
    unsigned short st_ino;
    unsigned short st_mode;
    short int st_nlink;
    short int st_uid;
    short int st_gid;
    short int st_rdev;
    long st_size;
    long st_atime;
    long st_mtime;
    long st_ctime;
};

/* Some common definitions. */
#define S_IFMT  0170000		/* type of file */
#define S_IFDIR 0040000  	/* directory */
#define S_IFCHR 0020000		/* character special */
#define S_IFBLK 0060000		/* block special */
#define S_IFREG 0100000		/* regular */
#define S_ISUID   04000		/* set user id on execution */
#define S_ISGID   02000		/* set group id on execution */
#define S_ISVTX   01000		/* save swapped text even after use */
#define S_IREAD   00400		/* read permission, owner */
#define S_IWRITE  00200		/* write permission, owner */
#define S_IEXEC   00100		/* execute/search permission, owner */
/* <include/stat.h> */



#endif
