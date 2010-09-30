#ifndef _MEM_H_
#define _MEM_H_

#include "task.h"

/* Pages of 4KB */
#define PAGESIZE 0x1000
#define NENTRIES 1024

typedef unsigned int pentry_t;
typedef unsigned int address_t;
typedef pentry_t * ptbl_t;

#define MEMSIZE_GB 4ul
#define MEMSIZE_BYTE MEMSIZE_GB * 1024ul * 1024ul * 1024ul
#define NPAGES ((MEMSIZE_BYTE) / PAGESIZE)
#define BITMAP_BYTESIZE (NPAGES / 8)
#define BITMAP_PAGESIZE (BITMAP_BYTESIZE / PAGESIZE)

/* First 8 MB are reserved for Kernel and Page allocation */
#define RESERVED_MEM ((address_t) 0x800000)

/* The firsts 4MB are for the kernel, the page directory and the kernel table.*/
/* The last page of the first 4MB will be the page directory */
#define KERNEL_AREA RESERVED_MEM - ((address_t) 0x400000)

#define SETBIT(X, P, V) (V ? ((1 << (P)) | (X)) : (~(1 << (P)) & X))

/* PRESENT, READ/WRITE AND SUPERVISOR BIT */
#define P_RW_SV 0x07
/* NOT PRESENT, READ/WRITE AND SUPERVISOR BIT */
#define NP_RW_SV 0x06

/* Creates the page directory with the firsts 4MB using Identity Mapping.
 * Fill the Page directory with the Kernel Table.
 * And sets the CR3 registry.
 * */
void _startPaging();

void _pageUp();
void _pageDown();



/* The following functions take a pointer to a Page Directory Entry, and fill
*   the structure with the given information 
*   */
pentry_t 	_pd_createEntry(void* address);
void        _pd_togglePresent(pentry_t*);

void* _reqpage(task_t task);

struct PagingNamespace {
	void (*start)();
    pentry_t (*createEntry) (void*);
    void (*togglePresent) (pentry_t*);
    
};

#endif
