#include "../include/driver.h"

//#if (CHIP == INTEL)
//#if ENABLE_AHA1540_SCSI && DMA_BUF_SIZE < 2048
/* A bit extra scratch for the Adaptec driver. */
#define BUF_EXTRA	(2048 - DMA_BUF_SIZE)
//#else
//#define BUF_EXTRA	0
//#endif

/* Claim space for variables. */
u8_t buffer[(unsigned) 2 * DMA_BUF_SIZE + BUF_EXTRA];
u8_t * tmp_buf;			/* the DMA buffer eventually */
phys_bytes tmp_phys;		/* phys address of DMA buffer */

//#else /* CHIP != INTEL */

/* Claim space for variables. */
//u8_t tmp_buf[DMA_BUF_SIZE];	/* the DMA buffer */
//phys_bytes tmp_phys;		/* phys address of DMA buffer */

//#endif /* CHIP != INTEL */

void init_buffer();
int do_rdwt(struct driver *dr, message *mp);
int do_vrdwt(struct driver *dr, message *mp);


/*===========================================================================*
 *				driver_task				     *
 *===========================================================================*/
void driver_task(dp)
struct driver *dp;	/* Device dependent entry points. */
{
/* Main program of any device driver task. */

  int r, caller, proc_nr;
  message mess;

  init_buffer();	/* Get a DMA buffer. */


  /* Here is the main loop of the disk task.  It waits for a message, carries
   * it out, and sends a reply.
   */
	while(TRUE);
 
}


/*===========================================================================*
 *				init_buffer				     *
 *===========================================================================*/
void init_buffer()
{
/* Select a buffer that can safely be used for dma transfers.  It may also
 * be used to read partition tables and such.  Its absolute address is
 * 'tmp_phys', the normal address is 'tmp_buf'.
 */

//#if (CHIP == INTEL)
  unsigned left;

  tmp_buf = buffer;
  tmp_phys = (phys_bytes)buffer;//vir2phys(buffer);

  if ((left = dma_bytes_left(tmp_phys)) < DMA_BUF_SIZE) {
	/* First half of buffer crosses a 64K boundary, can't DMA into that */
	tmp_buf += left;
	tmp_phys += left;
  }
//#else /* CHIP != INTEL */
//  tmp_phys = vir2phys(tmp_buf);
//#endif /* CHIP != INTEL */
}

void nop_cleanup(void){

}
int do_diocntl(struct driver *dr, message *m_ptr){

}
