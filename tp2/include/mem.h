#ifndef _MEM_H_
#define _MEM_H_

/* Pages of 4KB */
#define PAGESIZE 0x1000

/* Number of pages for 4GB*/
#define NUMPAGES 0x400

#define SETBIT(X, P, V) (V ? ((1 << (P)) | (X)) : (~(1 << (P)) & X))

typedef int pdentry_t;

/* The following functions take a pointer to a Page Directory Entry, and fill
   the structure with the given information */
pdentry_t   _pd_createEntry(void* address);
void        _pd_togglePresent(pdentry_t*);
void		_enablePaging();
#endif
