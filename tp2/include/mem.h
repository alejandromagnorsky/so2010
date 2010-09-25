#ifndef _MEM_H_
#define _MEM_H_

/* Pages of 4KB */
#define PAGESIZE 0x1000
#define NENTRIES 1024

typedef unsigned long pentry_t;
typedef unsigned long address_t;
typedef pentry_t * ptbl_t;

/* The firsts 4MB are for the kernel, the page directory and the kernel table.*/
/* The last page of the first 4MB will be the page directory */
#define KERNEL_AREA ((address_t) 0x400000)


#define SETBIT(X, P, V) (V ? ((1 << (P)) | (X)) : (~(1 << (P)) & X))

/* Creates the page directory with the firsts 4MB using Identity Mapping.
 * Fill the Page directory with the Kernel Table.
 * And sets the CR3 registry.
 * */
void _startPaging();


/* The following functions take a pointer to a Page Directory Entry, and fill
*   the structure with the given information 
*   */
pentry_t 	_pd_createEntry(void* address);
void        _pd_togglePresent(pentry_t*);

void*		_reqpage(void);

#endif
