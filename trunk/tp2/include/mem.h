#ifndef _MEM_H_
#define _MEM_H_

/* Pages of 4KB */
#define PAGESIZE 0x1000
#define NENTRIES 1024

/* The firsts 4MB are for the kernel, the page directory and the kernel table.*/
/* The last page of the first 4MB will be the page directory */
#define PD_DIR_POS (unsigned int*)0x3FF000
/* The predecessor page will be the Kernel Table, to do Indentity Mapping*/
#define KERNEL_TABLE_POS (unsigned int*) 0x3FE000 

typedef (unsigned int*) pdir_t;

#define SETBIT(X, P, V) (V ? ((1 << (P)) | (X)) : (~(1 << (P)) & X))
#define SET_PRESENT(X,V) SETBIT(X,0,V)
#define SET_RDWR(X,V) SETBIT(X,1,V)
#define SET_SUPERVISOR(X,V) SETBIT(X,2,V)

typedef int pdentry_t;

/* Creates the page directory with the firsts 4MB using Identity Mapping.
 * Fill the Page directory with the Kernel Table.
 * And sets the CR3 registry.
 * */
pdir_t _pd_create(void);

pdir_t _pd_createEmptyDirectoryPage();
void _setCR3(pdir_t directory_pos);
pdir_t _kernelTable();
void _setCR3(pdir_t page_directory);
void _enablePaging(pdir_t page_directory);


/* The following functions take a pointer to a Page Directory Entry, and fill
   the structure with the given information */
pdentry_t   _pd_createEntry(void* address);
void        _pd_togglePresent(pdentry_t*);

#endif
