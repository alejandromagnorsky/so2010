#include "../include/at_wini.h"

//#if ENABLE_WINI
struct hdmap {
	char *name;
	void (*task)();
} hdmap[] = {
	//#if ENABLE_AT_WINI
		{"at", createDriver_at},
	//#endif

	//#if ENABLE_BIOS_WINI
	//	{"bios", createDriver_bios},
	//#endif
};


struct driver w_dtab = {
  w_name,		/* current device's name */
  w_do_open,		/* open or mount request, initialize device */
  w_do_close,		/* release device */
  do_diocntl,		/* get or set a partition's geometry */
  w_prepare,		/* prepare for I/O on a given minor device */
  w_transfer,		/* do the I/O */
  nop_cleanup,		/* nothing to clean up */
  w_geometry,		/* tell the geometry of the disk */
};

void winchester_task(){

	char *hd = (char*)malloc(sizeof(char)*2);
	struct hdmap *map;
	
	/*k_getenv: Función a la que se le pasa una variable de entorno “name” y 
		devuelve un	puntero a la cadena con el valor de la variable.*/

	//hd = k_getenv("hd");
	hd = "at";
	for (map = hdmap; map < hdmap + sizeof(hdmap)/sizeof(hdmap[0]); map++) 
	{
		  if (hd == NULL || strcmp(hd, map->name) == 0) 
		  {
		        /*Ejecutar la tarea winchester seleccionada */
		        (*map->task)();
		  }
	}
	//panic("no hd driver", NO_NUM);

}

void createDriver_at()
{
	System.exec(at_winchester_task, "");
}

void createDriver_bios()
{
	System.exec(bios_winchester_task, "");
}


int at_winchester_task(char * a)
{
	System.name("Driver_at");
	printf("AT Driver\n");
	init_params();
	driver_task(&w_dtab);
}

int bios_winchester_task(char * a)
{

}

/*============================================================*
 * INIT_PARAMS                                                *
 * Esta rutina es llamada en el arranque para inicializar los *
 * parametros de las unidades                                 *
 *============================================================*/
void init_params()
{
	/* This routine is called at startup to initialize the drive parameters. */

	u16_t parv[2];
	unsigned int vector;
	int drive, nr_drives;
	struct wini *wn;
	u8_t params[16];
	phys_bytes param_phys = (phys_bytes)params;

	/* Get the number of drives from the BIOS data area */
	_memcpy((void *)0x475L, (void *)param_phys, 1L);
	if ((nr_drives = params[0]) > 2) nr_drives = 2;

	for (drive = 0, wn = wini; drive < MAX_DRIVES; drive++, wn++) {
		if (drive < nr_drives) {
			/* Copy the BIOS parameter vector */
			vector = drive == 0 ? WINI_0_PARM_VEC : WINI_1_PARM_VEC;
			_memcpy((void *)(vector * 4L), (void *)parv, 4L);

			/* Calculate the address of the parameters and copy them */
			_memcpy((void *)(hclick_to_physb(parv[1]) + parv[0]), (void *)param_phys, 16L);

			/* Copy the parameters to the structures of the drive */
			wn->lcylinders = bp_cylinders(params);
			wn->lheads = bp_heads(params);
			wn->lsectors = bp_sectors(params);
			wn->precomp = bp_precomp(params) >> 2;
		}
		wn->ldhpref = ldh_init(drive);
		wn->max_count = MAX_SECS << SECTOR_SHIFT;

		/* Base I/O register to address controller. */
		wn->base = drive < 2 ? REG_BASE0 : REG_BASE1;
	}
}

char * w_name(){

}

/*============================================================================*
 *				w_do_open				      *
 *============================================================================*/
int w_do_open(struct driver * dp, message * m_ptr){
/* Device open: Initialize the controller and read the partition table. */

  struct wini *wn;

  //if (w_prepare(m_ptr->DEVICE) == NIL_DEV) return(ENXIO);
  wn = w_wn;

  if (wn->state == 0) {
	/* Try to identify the device. */
	if (w_identify() != OK) {
		printf("%s: probe failed\n", w_name());
		if (wn->state & DEAF) w_reset();
		wn->state = 0;
		return(ENXIO);
	}
  }
  if (wn->open_ct == 0) {
#if ENABLE_ATAPI
	if (wn->state & ATAPI) {
		int r;

		if (m_ptr->COUNT & W_BIT) return(EACCES);
		if ((r = atapi_open()) != OK) return(r);
	}
#endif
	/* Partition the disk. */
	partition(&w_dtab, w_drive * DEV_PER_DRIVE, P_PRIMARY);
	wn->open_ct++;
  }
  return(OK);
}

/*===========================================================================*
 *				w_do_close				     *
 *===========================================================================*/
int  w_do_close(struct driver *dp, message *m_ptr){

}

/*===========================================================================*
 *				w_transfer				     *
 *===========================================================================*/
int  w_transfer(int proc_nr, int opcode, off_t position, iovec_t *iov, unsigned nr_req){

}

/*===========================================================================*
 *				w_geometry				     *
 *===========================================================================*/
void w_geometry(struct partition *entry){

}

/*===========================================================================*
 *				w_prepare				     *
 *===========================================================================*/

struct device *  w_prepare(int device){
	if (device < NR_DEVICES) {			/* d0, d0p[0-3], d1, ... */
		w_drive = device / DEV_PER_DRIVE;	/* save drive number */
		w_wn = &wini[w_drive];
		w_dv = &w_wn->part[device % DEV_PER_DRIVE];
	} else
	if ((unsigned) (device -= MINOR_d0p0s0) < NR_SUBDEVS) {/*d[0-7]p[0-3]s[0-3]*/
		w_drive = device / SUB_PER_DRIVE;
		w_wn = &wini[w_drive];
		w_dv = &w_wn->subpart[device % SUB_PER_DRIVE];
	} else {
		return(NIL_DEV);
	}
	return(w_dv);
}

/*===========================================================================*
 *				w_identify				     *
 *===========================================================================*/
int w_identify()
{

}

/*==========================================================================*
 *				w_handler				    *
 *==========================================================================*/
int w_handler(irq_hook_t* hook)
{

}

/*===========================================================================*
 *				w_reset					     *
 *===========================================================================*/
int w_reset()
{

}

/*===========================================================================*
 *				atapi_open				     *
 *===========================================================================*/
int atapi_open()
{

}

//#endif /* ENABLE_WINI */
