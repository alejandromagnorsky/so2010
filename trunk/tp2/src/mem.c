#include "../include/mem.h"

/* Private memory functions */
void _pd_createDirectoryTbl();
void _setCR3();
void _enablePaging();
void _set_Entry(pentry_t * entry, address_t address, unsigned int permissions);
address_t _getFreePage();
int _checkPageStatus(address_t address);
void _setPageUsed(address_t address);
void _setPageUnused(address_t address);


/* Directory Table */
static ptbl_t directoryTbl;

static address_t tablesArea = KERNEL_AREA;

/* Free pages bitmap */
static char * pageMap;
static address_t lastPageDelivered;

/* Namespace structure */
struct PagingNamespace Paging =
{     _startPaging,
	  _pd_createEntry,
	  _pd_togglePresent
};


void _setEntry(pentry_t * entry, address_t address, unsigned int permissions){
	address &= 0xFFFFF000;
	*entry = address;
	*entry |= permissions;
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
	
	for(i = 0; i < NENTRIES; i++){
		address = 0;
		tbl = (ptbl_t) (tablesArea + i * PAGESIZE);
		_setEntry(&directoryTbl[i], (address_t) tbl, P_RW_SV);
		// VER SI CALCULAMOS LA MEMORIA Y ASIGNAMOS SOLO LA NECESARIA.
		for(j = 0; j < NENTRIES; j++){
			_setEntry(&tbl[j], address, P_RW_SV);
			address += PAGESIZE;
		}
	}
	
	/* Below the directory will be the bitmap */
	pageMap = (char *) KERNEL_AREA - PAGESIZE - BITMAP_PAGESIZE * PAGESIZE;
	for(i=0; i < BITMAP_PAGESIZE; i++){
		pageMap[i] = ((i * PAGESIZE) < RESERVED_MEM) ? 1 : 0;
	}
	lastPageDelivered = RESERVED_MEM - PAGESIZE;
}

int _pageUp(void * pg){
	address_t address = (address_t) pg;
	//printf("pgUp_address: %d\n", address);
	ptbl_t tbl = (ptbl_t) tablesArea + GETDIRENTRY(address) * PAGESIZE;		
	int i = 0;
	if(!ISPRESENT(directoryTbl[GETDIRENTRY(address)])){
		directoryTbl[GETDIRENTRY(address)] |= PRESENT;
		for(i = 0; i < NENTRIES; i++){
			_setEntry(&tbl[i], address, i == 0 ? P_RW_SV : NP_RW_SV);
			address += PAGESIZE;
		}
	}else{
		tbl[GETTBLENTRY(address)] |= PRESENT;
	}
}

int _pageDown(void * pg){
	address_t address = (address_t) pg;
	//printf("pgDn_address: %d\n", address);
	ptbl_t tbl = (ptbl_t) tablesArea + GETDIRENTRY(address) * PAGESIZE;
	int i = 0;
	tbl = (ptbl_t) tablesArea + GETDIRENTRY(address) * PAGESIZE;
	tbl[GETTBLENTRY(address)] ^= PRESENT;
	address += PAGESIZE;
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
	address_t possiblePage = (lastPageDelivered + PAGESIZE);
	int i = 0;
	for(i = 0; i < NPAGES; i++){
		if(!_checkPageStatus(possiblePage)){
			_setPageUsed(possiblePage);
			_pageUp((void*) possiblePage);
			//printf("FreePage: %d\n", possiblePage);
			return lastPageDelivered = possiblePage;
		}
		possiblePage = (possiblePage + PAGESIZE);
	}
	return NULL;
}

int _setFreePage(void * pg){
	address_t address = *((address_t *) pg);
	if(!(address % PAGESIZE)){
		return ERROR_ILLEGALPAGE;
	}
	_pageDown(pg);
	_setPageUnused(address);
	lastPageDelivered = address - PAGESIZE;
}

int _checkPageStatus(address_t address){
	int nbyte = address / PAGESIZE;
	int nbit = address % 8;
	char currByte = pageMap[nbyte];
	return (currByte >> nbit) & 1;
}

void _setPageUsed(address_t address){
	int nbyte = address / PAGESIZE;
	int nbit = address % 8;
	pageMap[nbyte] |= (1 << nbit);
}

void _setPageUnused(address_t address){
	int nbyte = address / PAGESIZE;
	int nbit = address % 8;
	pageMap[nbyte] &= ~(1 << nbit);
}

void* _reqpage(task_t task){
	return (void *) _getFreePage();
}

void* _sys_malloc(size_t size){
	return (void*) _getFreePage();
}

void _sys_free(void *pointer){
	_setFreePage(pointer);
}
