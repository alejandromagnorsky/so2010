/* Types and constants shared between the generic and device dependent
 * device driver code.
 */


#ifndef DRIVER_H_
#define DRIVER_H_

/*#include <minix/callnr.h>
#include <minix/com.h>
#include "proc.h"
#include <minix/partition.h>
#include <minix/u64.h>*/
#include "partitions.h"
#include "defs.h"


/* <COM.H> */
#define DEVICE         m2_i1	/* major-minor device */
#define PROC_NR        m2_i2	/* which (proc) wants I/O? */
#define COUNT          m2_i3	/* how many bytes to transfer */
#define REQUEST        m2_i3	/* ioctl request code */
#define POSITION       m2_l1	/* file offset */
#define ADDRESS        m2_p1	/* core buffer address */

/* Types relating to messages. */
#define M1                 1
#define M3                 3
#define M4                 4
#define M3_STRING         14

/* </COM.H> */


/* <TYPE.H> */


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
#define m4_l5  m_u.m_m4.m4l5

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




typedef unsigned int vir_bytes;	/* virtual addresses and lengths in bytes */

typedef unsigned long off_t;
typedef unsigned long phys_bytes;

typedef struct {int m1i1, m1i2, m1i3; char *m1p1, *m1p2, *m1p3;} mess_1;
typedef struct {int m2i1, m2i2, m2i3; long m2l1, m2l2; char *m2p1;} mess_2;
typedef struct {int m3i1, m3i2; char *m3p1; char m3ca1[M3_STRING];} mess_3;
typedef struct {long m4l1, m4l2, m4l3, m4l4, m4l5;} mess_4;
typedef struct {char m5c1, m5c2; int m5i1, m5i2; long m5l1, m5l2, m5l3;}mess_5;



typedef struct {
  int m_source;			/* who sent the message */
  int m_type;			/* what kind of message is it */
  union {
	mess_1 m_m1;
	mess_2 m_m2;
	mess_3 m_m3;
	mess_4 m_m4;
	mess_5 m_m5;
  } m_u;
} message;
/* </TYPE.H> */


typedef struct {
  vir_bytes iov_addr;		/* address of an I/O buffer */
  vir_bytes iov_size;		/* sizeof an I/O buffer */
} iovec_t;


/* Base and size of a partition in bytes. */
struct device {
  u64_t dv_base;
  u64_t dv_size;
};

/* To translate an address in kernel space to a physical address.  This is
 * the same as umap(proc_ptr, D, vir, sizeof(*vir)), but a lot less costly.
 */
//#define vir2phys(vir)	(data_base + (vir_bytes) (vir))



/* Info about and entry points into the device dependent code. */
struct driver {
  char *(*dr_name)(void);
  int (*dr_open)(struct driver *dp, message *m_ptr);
  int (*dr_close)(struct driver *dp, message *m_ptr);
  int (*dr_ioctl)(struct driver *dp, message *m_ptr);
  struct device *(*dr_prepare)(int device);
  int (*dr_transfer)(int proc_nr, int opcode, off_t position, iovec_t *iov, unsigned nr_req);
  void (*dr_cleanup)(void);
  void (*dr_geometry)(struct partition *entry);
};

//#if (CHIP == INTEL)

/* Number of bytes you can DMA before hitting a 64K boundary: */
#define dma_bytes_left(phys)    \
   ((unsigned) (sizeof(int) == 2 ? 0 : 0x10000) - (unsigned) ((phys) & 0xFFFF))

//#endif /* CHIP == INTEL */


#define NIL_DEV		((struct device *) 0)

/* Functions defined by driver.c: */
void driver_task(struct driver *dr);
char *no_name(void);
int do_nop(struct driver *dp, message *m_ptr);
struct device *nop_prepare(int device);
void nop_cleanup(void);
int do_diocntl(struct driver *dr, message *m_ptr);

/* Parameters for the disk drive. */
#define SECTOR_SIZE      512	/* physical sector size in bytes */
#define SECTOR_SHIFT       9	/* for division */
#define SECTOR_MASK      511	/* and remainder */

/* Size of the DMA buffer buffer in bytes. */
#define DMA_SECTORS 	1
#define DMA_BUF_SIZE	(DMA_SECTORS * SECTOR_SIZE)

//#if (CHIP == INTEL)
extern u8_t *tmp_buf;			/* the DMA buffer */
//#else
//extern u8_t tmp_buf[];			/* the DMA buffer */
//#endif
extern phys_bytes tmp_phys;		/* phys address of DMA buffer */

#endif
