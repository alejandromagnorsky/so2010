#ifndef _WINI_H_
#define _WINI_H_

#include "klib.h"
#include "driver.h"
#include "io.h"
#include "errno.h"
#include "drvlib.h"
#include "type.h"
#include "config.h"
#include "minixasm.h"

#define ATAPI_DEBUG	    0	/* To debug ATAPI code. */


/* CONST.H */
#define I_TYPE          0170000	/* this field gives inode type */
#define I_REGULAR       0100000	/* regular file, not dir or special */
#define I_BLOCK_SPECIAL 0060000	/* block special file */
#define I_DIRECTORY     0040000	/* file is a directory */
#define I_CHAR_SPECIAL  0020000	/* character special file */
#define I_NAMED_PIPE	0010000 /* named pipe (FIFO) */
#define I_SET_UID_BIT   0004000	/* set effective uid_t on exec */
#define I_SET_GID_BIT   0002000	/* set effective gid_t on exec */
#define ALL_MODES       0006777	/* all bits for user, group and others */
#define RWX_MODES       0000777	/* mode bits for RWX only */
#define R_BIT           0000004	/* Rwx protection bit */
#define W_BIT           0000002	/* rWx protection bit */
#define X_BIT           0000001	/* rwX protection bit */
#define I_NOT_ALLOC     0000000	/* this inode is free */

/* CURSES.H */
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef ERR
#define ERR (-1)	/* general error flag */
#endif
#ifndef OK
#define OK 0		/* general OK flag */
#endif


/* I/O Ports used by winchester disk controllers. */

/* Read and write registers */
#define REG_BASE0	0x1F0	/* base register of controller 0 */
#define REG_BASE1	0x170	/* base register of controller 1 */
#define REG_DATA	    0	/* data register (offset from the base reg.) */
#define REG_PRECOMP	    1	/* start of write precompensation */
#define REG_COUNT	    2	/* sectors to transfer */
#define REG_SECTOR	    3	/* sector number */
#define REG_CYL_LO	    4	/* low byte of cylinder number */
#define REG_CYL_HI	    5	/* high byte of cylinder number */
#define REG_LDH		    6	/* lba, drive and head */
#define   LDH_DEFAULT		0xA0	/* ECC enable, 512 bytes per sector */
#define   LDH_LBA		0x40	/* Use LBA addressing */
#define   ldh_init(drive)	(LDH_DEFAULT | ((drive) << 4))

/* Read only registers */
#define REG_STATUS	    7	/* status */
#define   STATUS_BSY		0x80	/* controller busy */
#define	  STATUS_RDY		0x40	/* drive ready */
#define	  STATUS_WF		0x20	/* write fault */
#define	  STATUS_SC		0x10	/* seek complete (obsolete) */
#define	  STATUS_DRQ		0x08	/* data transfer request */
#define	  STATUS_CRD		0x04	/* corrected data */
#define	  STATUS_IDX		0x02	/* index pulse */
#define	  STATUS_ERR		0x01	/* error */
#define	  STATUS_ADMBSY	       0x100	/* administratively busy (software) */
#define REG_ERROR	    1	/* error code */
#define	  ERROR_BB		0x80	/* bad block */
#define	  ERROR_ECC		0x40	/* bad ecc bytes */
#define	  ERROR_ID		0x10	/* id not found */
#define	  ERROR_AC		0x04	/* aborted command */
#define	  ERROR_TK		0x02	/* track zero error */
#define	  ERROR_DM		0x01	/* no data address mark */

/* Write only registers */
#define REG_COMMAND	    7	/* command */
#define   CMD_IDLE		0x00	/* for w_command: drive idle */
#define   CMD_RECALIBRATE	0x10	/* recalibrate drive */
#define   CMD_READ		0x20	/* read data */
#define   CMD_WRITE		0x30	/* write data */
#define   CMD_READVERIFY	0x40	/* read verify */
#define   CMD_FORMAT		0x50	/* format track */
#define   CMD_SEEK		0x70	/* seek cylinder */
#define   CMD_DIAG		0x90	/* execute device diagnostics */
#define   CMD_SPECIFY		0x91	/* specify parameters */
#define   ATA_IDENTIFY		0xEC	/* identify drive */
#define REG_CTL		0x206	/* control register */
#define   CTL_NORETRY		0x80	/* disable access retry */
#define   CTL_NOECC		0x40	/* disable ecc retry */
#define   CTL_EIGHTHEADS	0x08	/* more than eight heads */
#define   CTL_RESET		0x04	/* reset controller */
#define   CTL_INTDISABLE	0x02	/* disable interrupts */

#if ENABLE_ATAPI
#define   ERROR_SENSE           0xF0    /* sense key mask */
#define     SENSE_NONE          0x00    /* no sense key */
#define     SENSE_RECERR        0x10    /* recovered error */
#define     SENSE_NOTRDY        0x20    /* not ready */
#define     SENSE_MEDERR        0x30    /* medium error */
#define     SENSE_HRDERR        0x40    /* hardware error */
#define     SENSE_ILRQST        0x50    /* illegal request */
#define     SENSE_UATTN         0x60    /* unit attention */
#define     SENSE_DPROT         0x70    /* data protect */
#define     SENSE_ABRT          0xb0    /* aborted command */
#define     SENSE_MISCOM        0xe0    /* miscompare */
#define   ERROR_MCR             0x08    /* media change requested */
#define   ERROR_ABRT            0x04    /* aborted command */
#define   ERROR_EOM             0x02    /* end of media detected */
#define   ERROR_ILI             0x01    /* illegal length indication */
#define REG_FEAT            1   /* features */
#define   FEAT_OVERLAP          0x02    /* overlap */
#define   FEAT_DMA              0x01    /* dma */
#define REG_IRR             2   /* interrupt reason register */
#define   IRR_REL               0x04    /* release */
#define   IRR_IO                0x02    /* direction for xfer */
#define   IRR_COD               0x01    /* command or data */
#define REG_SAMTAG          3
#define REG_CNT_LO          4   /* low byte of cylinder number */
#define REG_CNT_HI          5   /* high byte of cylinder number */
#define REG_DRIVE           6   /* drive select */
#define REG_STATUS          7   /* status */
#define   STATUS_BSY            0x80    /* controller busy */
#define   STATUS_DRDY           0x40    /* drive ready */
#define   STATUS_DMADF          0x20    /* dma ready/drive fault */
#define   STATUS_SRVCDSC        0x10    /* service or dsc */
#define   STATUS_DRQ            0x08    /* data transfer request */
#define   STATUS_CORR           0x04    /* correctable error occurred */
#define   STATUS_CHECK          0x01    /* check error */

#define   ATAPI_PACKETCMD       0xA0    /* packet command */
#define   ATAPI_IDENTIFY        0xA1    /* identify drive */
#define   SCSI_READ10           0x28    /* read from disk */

#define CD_SECTOR_SIZE		2048	/* sector size of a CD-ROM */
#endif /* ATAPI */

/* Interrupt request lines. */
#define NO_IRQ		 0	/* no IRQ set yet */
#define AT_IRQ0		0x2E	/* interrupt number for controller 0 */
#define AT_IRQ1		0x2F	/* interrupt number for controller 1 */

/* Common command block */
struct command {
  u8_t	precomp;	/* REG_PRECOMP, etc. */
  u8_t	count;
  u8_t	sector;
  u8_t	cyl_lo;
  u8_t	cyl_hi;
  u8_t	ldh;
  u8_t	command;
};


/* Error codes */
#define ERR		 (-1)	/* general error */
#define ERR_BAD_SECTOR	 (-2)	/* block marked bad detected */

/* Some controllers don't interrupt, the clock will wake us up. */
#define WAKEUP		(32*HZ)	/* drive may be out for 31 seconds max */

/* Miscellaneous. */
#define MAX_DRIVES         4	/* this driver supports 4 drives (d0 - d3) */
#if _WORD_SIZE > 2
#define MAX_SECS	 256	/* controller can transfer this many sectors */
#else
#define MAX_SECS	 127	/* but not to a 16 bit process */
#endif
#define MAX_ERRORS         4	/* how often to try rd/wt before quitting */
#define NR_DEVICES      (MAX_DRIVES * DEV_PER_DRIVE)
#define SUB_PER_DRIVE	(NR_PARTITIONS * NR_PARTITIONS)
#define NR_SUBDEVS	(MAX_DRIVES * SUB_PER_DRIVE)
#define TIMEOUT      5000000	/* controller timeout in us */
#define RECOVERYTIME  500000	/* controller recovery time in us */
#define INITIALIZED	0x01	/* drive is initialized */
#define DEAF		0x02	/* controller must be reset */
#define SMART		0x04	/* drive supports ATA commands */

#define NR_PARTITIONS	4			/* number of entries in partition table */
#define DEV_PER_DRIVE	(1 + NR_PARTITIONS)
#define WINI_0_PARM_VEC 0x41
#define WINI_1_PARM_VEC 0x46

#define HCLICK_SHIFT       4	/* log2 of HCLICK_SIZE */
#define HCLICK_SIZE       16	/* hardware segment conversion magic */
#define hclick_to_physb(n) ((phys_bytes) (n) << HCLICK_SHIFT)
#define bp_cylinders(t)		(* (u16_t *) (&(t)[0]))
#define bp_heads(t)		(* (u8_t *)  (&(t)[2]))
#define bp_reduced_wr(t)	(* (u16_t *) (&(t)[3]))
#define bp_precomp(t)		(* (u16_t *) (&(t)[5]))
#define bp_max_ecc(t)		(* (u8_t *)  (&(t)[7]))
#define bp_ctlbyte(t)		(* (u8_t *)  (&(t)[8]))
#define bp_landingzone(t)	(* (u16_t *) (&(t)[12]))
#define bp_sectors(t)		(* (u8_t *)  (&(t)[14]))



#if ENABLE_ATAPI
#define ATAPI		0x08	/* it is an ATAPI device */
#else
#define ATAPI		   0	/* don't bother with ATAPI; optimise out */
#endif


/* Variables. */
struct wini {		/* main drive struct, one entry per drive */
  unsigned state;		/* drive state: deaf, initialized, dead */
  unsigned base;		/* base register of the register file */
  unsigned irq;			/* interrupt request line */
  unsigned lcylinders;		/* logical number of cylinders (BIOS) */
  unsigned lheads;		/* logical number of heads */
  unsigned lsectors;		/* logical number of sectors per track */
  unsigned pcylinders;		/* physical number of cylinders (translated) */
  unsigned pheads;		/* physical number of heads */
  unsigned psectors;		/* physical number of sectors per track */
  unsigned ldhpref;		/* top four bytes of the LDH (head) register */
  unsigned precomp;		/* write precompensation cylinder / 4 */
  unsigned max_count;		/* max request for this drive */
  unsigned open_ct;		/* in-use count */
  irq_hook_t hook;		/* interrupt hook */
  struct device part[DEV_PER_DRIVE];	/* disks and partitions */
  struct device subpart[SUB_PER_DRIVE];	/* subpartitions */
} wini[MAX_DRIVES], *w_wn;

int win_tasknr;			/* my task number */
int w_command;			/* current command in execution */
int w_status;			/* status after interrupt */
int w_drive;			/* selected drive */
struct device *w_dv;		/* device's base and size */
timer_t w_tmr_timeout;		/* timer for w_timeout */


extern struct system_t System;

void	createDriver_at();
void 	createDriver_bios();
void 	winchester_task();
int 	at_winchester_task();
int 	bios_winchester_task();
void 	init_params();
char* 	w_name();
int 	w_do_open(struct driver * dp, message * m_ptr);
int  	w_do_close(struct driver *dp, message *m_ptr);
struct 	device *  w_prepare(int device);
int 	w_identify();
int 	w_handler(irq_hook_t* hook);
int  	w_transfer(int proc_nr, int opcode, off_t position, iovec_t *iov, unsigned nr_req);
void  	w_geometry(struct partition *entry);


/* i8259.c */
void put_irq_handler(irq_hook_t *hook, int irq, irq_handler_t handler);

#endif
