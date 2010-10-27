#include "../include/mem.h"

/* Private memory functions */
void _pd_createDirectoryTbl(int kbytes);
void _setCR3();
void _enablePaging();
void _set_Entry(pentry_t * entry, address_t address, unsigned int permissions);
address_t _getFreePages(int);
int _checkPageStatus(address_t address);
void _setPageUsed(address_t address);
void _setPageUnused(address_t address);
void _initializeMemMan();

void* _malloc(block_t block, size_t size);


/* Directory Table */
static ptbl_t directoryTbl;

static address_t tablesArea = KERNEL_AREA;

/* Memory info */
unsigned int totalbytes = 0;

/* Free pages bitmap */
static char * pageMap;
static address_t lastPageDelivered;

/* Namespace structure */
struct PagingNamespace Paging =
{     
	_startPaging,
	_pageUp,
	_pageDown,
};


void _setEntry(pentry_t * entry, address_t address, unsigned int permissions){
	address &= 0xFFFFF000;
	*entry = address;
	*entry |= permissions;
}

void _startPaging(int kbytes){
	_pd_createDirectoryTbl(kbytes);
	_setCR3();
	_enablePaging();
}


void _pd_createDirectoryTbl(int kbytes){
	directoryTbl = (ptbl_t) KERNEL_AREA - PAGESIZE;
	ptbl_t tbl;
	int i = 0, j = 0;
	address_t address = 0;
	
	totalbytes = (kbytes * 1024);
	unsigned int totalpages = totalbytes / PAGESIZE;
	unsigned int totaltbls = totalpages / NENTRIES;

	for(i=0; i < totaltbls; i++){
		tbl = (ptbl_t)(tablesArea + i * PAGESIZE);
		_setEntry(&directoryTbl[i], (address_t) tbl, P_RW_SV);
		for(j = 0; j < NENTRIES && (i * NENTRIES + j) < totalpages ; j++){
			_setEntry(&tbl[j], address, P_RW_SV);
			address += PAGESIZE;
		}
		if(j < NENTRIES){
			for( ; j < NENTRIES; j++){
				_setEntry(&tbl[j], address, NP_RW_SV);
				address += PAGESIZE;
			}
		}
	}
	for( ; i < NENTRIES; i++){
		tbl = (ptbl_t) (tablesArea + i * PAGESIZE);
	}

	_initializeMemMan();
}


void _initializeMemMan(){
	pageMap = (char *) KERNEL_AREA - PAGESIZE - BITMAP_BYTESIZE - PAGESIZE * 3;
	int i = 0;
	for(i = 0; i < BITMAP_BYTESIZE ; i++){
		pageMap[i] = ((i * PAGESIZE) < RESERVED_MEM || (i * PAGESIZE) > totalbytes) ? 0xFF : 0x00;
	}

	lastPageDelivered = RESERVED_MEM - PAGESIZE;	
}


int _pageUp(void * pg){
	address_t address = (address_t) pg;
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
	ptbl_t tbl = (ptbl_t) tablesArea + GETDIRENTRY(address) * PAGESIZE;
	int i = 0;
	tbl = (ptbl_t) tablesArea + GETDIRENTRY(address) * PAGESIZE;
	//printf("ENTRY: %d\n", tbl[GETTBLENTRY(address)]); 
	tbl[GETTBLENTRY(address)] ^= PRESENT;
	//printf("ENTRY: %d\n", tbl[GETTBLENTRY(address)]);	
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

address_t _getFreePages(int npages){
	address_t possiblePage = (lastPageDelivered + PAGESIZE) % totalbytes;
	address_t auxAddress = possiblePage;
	int checks = 0, satChecks = 0, lastNotCheck = 0, i = 0;

	if(npages > (totalbytes / PAGESIZE)){
		return NULL;
	}

	for(i = 0; i < NPAGES; i++){
		for(checks = 0, satChecks = 0; checks < npages; checks++){
			if(!_checkPageStatus(auxAddress)){
				satChecks++;
			}else{
				lastNotCheck = checks;
			}
			auxAddress = (auxAddress + PAGESIZE) % totalbytes; 
		}
		if(satChecks == npages){
			for(i = 0; i < npages; i++){
				auxAddress = ((possiblePage + i * PAGESIZE) % totalbytes);
				_setPageUsed(auxAddress);
				_pageUp((void*)auxAddress);	
			}
			lastPageDelivered = auxAddress - PAGESIZE;
			return possiblePage;
		}else{
			i -= (8 - lastNotCheck);
			possiblePage = (possiblePage + PAGESIZE * (lastNotCheck + 1));
		}
	}	

	return NULL;
}

int _setFreePage(void * pg, int npages){
	printf("Address to Free: %d\n", (unsigned int) pg);
	address_t address = *((address_t *) pg);
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
	return (void *) _getFreePages(1);
}

void* _sys_malloc(size_t size){
	int npages = size / PAGESIZE + ((size % PAGESIZE) ? 1 : 0);
	return (void*) _getFreePages(npages);
}

void _sys_free(void *pointer, int npages){
	int i = 0;
	for( i = 0; i < npages; i++){
		_setPageUnused(((address_t) pointer) + i * PAGESIZE);
	}
}

/*
struct block_t {
	struct block_t * next;
	void * firstPage;
	unsigned int npages;
	unsigned int freeSpace;
	void * freeMemory;
};
*/
void* malloc(size_t size){
	block_t newBlock = NULL;
	int npages = 0;
	printf("SYSTEM.TASK = %d\n", System.task);
	if(System.task->mem == NULL){
		newBlock = _sys_malloc(size);
		if(newBlock == NULL){
			return NULL;
		}
		newBlock->next = NULL;
		newBlock->npages = size / PAGESIZE + ((size % PAGESIZE) ? 1 : 0);
		newBlock->freeSpace = newBlock->npages * PAGESIZE - sizeof(struct block_t);
		newBlock->ptrFreeMemory = newBlock + sizeof(struct block_t); 
		System.task->mem = newBlock;
		return (void*) _malloc(newBlock->next, size);
	}else{
		return (void*) _malloc(System.task->mem, size);
	}	
}

void* _malloc(block_t block, size_t size){
	
	void * ret = NULL;
	block_t newBlock = NULL; 

	if(size > block->freeSpace){
		if(block->next == NULL){
			printf("pide nuevo bloque\n");
			newBlock = _sys_malloc(size);
			if(newBlock == NULL){
				return NULL;
			}
			newBlock->next = NULL;
			newBlock->npages = size / PAGESIZE + ((size % PAGESIZE) ? 1 : 0);
			newBlock->freeSpace = newBlock->npages * PAGESIZE - sizeof(struct block_t);
			newBlock->ptrFreeMemory = newBlock + sizeof(struct block_t); 
			block->next = newBlock;
			return _malloc(block->next, size);
		}else{
			return _malloc(block->next, size);
		}
	}else{
		ret = block->ptrFreeMemory;
		block->ptrFreeMemory += size;
		block->freeSpace -= size;
		return ret;
	}

}



void free(void *pointer){
	return _sys_free(pointer, 1);
}
