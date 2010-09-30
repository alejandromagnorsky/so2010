#include "../include/mem.h"

/* Private memory functions */
void _pd_createDirectoryTbl();
void _setCR3();
void _enablePaging();
void _setEntry(pentry_t * entry, int address);
address_t _getFreePage();
int checkPageStatus(address_t);
void setPageUsed(address_t);
void setPageUnused(address_t);


/* Directory Table */
static ptbl_t directoryTbl;

/* Free pages bitmap */
static char * pageMap;
static address_t lastPageDelivered;

/* Namespace structure */
struct PagingNamespace Paging =
{     _startPaging,
	  _pd_createEntry,
	  _pd_togglePresent
};



void _setEntry(pentry_t * entry, int address){
	address &= 0xFFFFF000;
	*entry = address;
}

pentry_t _pd_createEntry(void* address) {
    return ((int) address << 12) & 0xFFFFF000;
}

void _pd_togglePresent(pentry_t * entry) {
    *entry ^= 1;
    return;
}


void _startPaging(){
	_pd_createDirectoryTbl();
	_setCR3();
	_enablePaging();
}

void _pd_createDirectoryTbl(){
	directoryTbl = (ptbl_t) KERNEL_AREA - PAGESIZE;
	ptbl_t tbl;
	int i = 0, j = 0;
	address_t address = 0;
	
	for(i = 0; i < NENTRIES; i++)
	{
		address = 0;
		tbl = (ptbl_t) KERNEL_AREA + i * PAGESIZE;
		_setEntry(&directoryTbl[i], (address_t) tbl);
		(i == 0 || i == 1) ? (directoryTbl[i] |= P_RW_SV) : (directoryTbl[i] |= NP_RW_SV);
		if(i == 0){
			for(j = 0; j < NENTRIES; j++){
				_setEntry(&tbl[j], address);
				(i == 0 || i == 1) ? (tbl[j] |= P_RW_SV) : (tbl[j] |= NP_RW_SV);
				address += PAGESIZE;
			}
		}
	}
	
	/* Below the directory will be the bitmap */
	pageMap = (char *) KERNEL_AREA - PAGESIZE - BITMAP_PAGESIZE * PAGESIZE;
	for(i = 0; i < BITMAP_BYTESIZE; i++){
		pageMap[i] = (i * PAGESIZE < RESERVED_MEM) ? 1 : 0;
	}
	lastPageDelivered = RESERVED_MEM - PAGESIZE;
}

void _pageUp(){
	
}

void _pageDown(){
	
}

void _setCR3(){
	//Set the page_directory address in CR3
	asm volatile("MOVL 	%0, %%CR3" : : "b" (directoryTbl));
}

void _enablePaging(){
	asm volatile("MOVL	%CR0, %EAX"); 			// Get the value of CR0.
	asm volatile("OR	$0x80000000, %EAX");	// Set PG bit.
	asm volatile("MOVL 	%EAX, %CR0");			// Set CR0 value.
}

address_t _getFreePage(){
	address_t possiblePage = (lastPageDelivered + PAGESIZE) % MEMSIZE_BYTE;
	int i = 0;
	for(i = 0; i < NPAGES; i++){
		if(checkPageStatus(possiblePage)){
			setPageUsed(possiblePage);
			return lastPageDelivered = possiblePage;
		}
		possiblePage = (lastPageDelivered + PAGESIZE) % MEMSIZE_BYTE;
	}
	return NULL;
}

void* _reqpage(task_t task){

}

int checkPageStatus(address_t address){
	int nbyte = address / 8;
	int nbit = address % 8;
	char currByte = pageMap[nbyte];
	return (currByte >> nbit) & 1;
}

void setPageUsed(address_t address){
	int nbyte = address / 8;
	int nbit = address % 8;
	pageMap[nbyte] |= (1 << nbit);
}

void setPageUnused(address_t address){
	int nbyte = address / 8;
	int nbit = address % 8;
	pageMap[nbyte] &= ~(1 << nbit);
}

