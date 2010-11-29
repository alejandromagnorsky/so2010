#include "../include/file.h"
#include "../include/at_wini.h"
#include "../include/klib.h"


File fileTable[_MAX_FILES];
char sectorTable[_MAX_SECTOR_BYTES] = {0};


File * current_dir = NULL;


void __initFileTable();

void __printFile(File * f);

void __printFileTreeTabs( File *f, int tabs);


void __loadFileTable();

void __addFileChild(File * parent, File * child);


void clearSector(int sector){
	char bit = sector%8;
	sectorTable[sector/8] &= ~(1 << bit);
	return;
}

void setSector(int sector){
	char bit = sector%8;
	sectorTable[sector/8]|= 1 << bit;
	return;
}

int getSector(int sector){
	char bit = sector%8;
	return sectorTable[sector/8] & (1<<bit);
}

void exportTable(){
	disk_cmd headerCMD = {ATA0, _TABLE_STARTUP_SECTOR, 0, strlen(_TABLE_HEADER), _TABLE_HEADER };
	disk_cmd cmd = {ATA0, _TABLE_STARTUP_SECTOR, strlen(_TABLE_HEADER), _MAX_SECTOR_BYTES, sectorTable };
	System.writeDisk(&headerCMD);
	System.writeDisk(&cmd);
}


void createFilesystem(){
	int i;

	// First chunk is protected
	for(i=0;i<_TABLE_STARTUP_SECTOR;i++)
		setSector(i);

	// Sector table is protected
	for(i=0;i<_TABLE_SECTOR_SIZE;i++)
		setSector(_TABLE_STARTUP_SECTOR+i);

	exportTable();

	__initFileTable();
}


void printTable(int from, int to){
	int i;
	for(i=from;i<to;i++){
		int exists = getSector(i);
		printf("Sector %d: %s\n", i, exists ? "exists" : "does not exist");
	}
}	

int loadFileSystem(){

	char header[100];
	disk_cmd headerCMD = {ATA0, _TABLE_STARTUP_SECTOR, 0, strlen(_TABLE_HEADER), header };
	disk_cmd cmd = {ATA0, _TABLE_STARTUP_SECTOR, strlen(_TABLE_HEADER), _MAX_SECTOR_BYTES, sectorTable };

	System.readDisk(&headerCMD);
	System.readDisk(&cmd);


	if(!strcmp(header,_TABLE_HEADER)){
	//	__loadFileTable();
		return 0;
	}

	return -1;
}


int __getFileFreeSector(){
	int i;
	for(i=_FILETABLE_SECTOR_START;i<_MAX_SECTORS;i++)
		if(!getSector(i))
			return i;
	return -1;
}

int getFreeSector(){
	int i;
	for(i=0;i<_MAX_SECTORS;i++)
		if(!getSector(i))
			return i;
	return -1;
}


int __getFileIndexBySector(int sector){
	int i;
	for(i=0;i<_MAX_FILES;i++)
		if(fileTable[i].sector == sector)
			return i;
	return -1;
}

void __freeFile(int index){
	fileTable[index].sector = -1;
}

File * getFreeFile(){
	int i;
	for(i=0;i<_MAX_FILES;i++)
		if(fileTable[i].sector == -1)
			return fileTable+i;
	return NULL;
}


File * __getCurrentDir(){
	return current_dir;
}


void __setCurrentDir( File * f){
	current_dir = f;
}

File * __loadFile(int sector){
	// If it isnt mapped yet...
	if(!getSector(sector))
		return NULL;

	File * file = getFreeFile();


	int i;
	for(i=0;i<_FILE_CHILDREN;i++)
		file->children[i] = NULL;
	

	file->sector = sector;

	// FILE HEADER
	// 255 bytes: name
	// 4 bytes: size
	// 4 bytes: parent sector
	// FILE FOOTER

	int base = 0;

	char header[50];
	char footer[50];

	// Now prepare to read file header on sector
	disk_cmd fileHeader = {ATA0, file->sector, base, strlen(_FILE_HEADER)+1,header};
	base += strlen(_FILE_HEADER)+1;

	disk_cmd fileName = {ATA0, file->sector, base, _FILE_NAMELENGTH, file->name};
	base += _FILE_NAMELENGTH;

	disk_cmd fileSize = {ATA0, file->sector,base, 4,(char*)(&(file->size))};
	base += sizeof(int);

	disk_cmd fileParent = {ATA0, file->sector, base, sizeof(int), (char *)(&file->psector)};
	base += sizeof(int);

	file->parent = NULL;

	disk_cmd fileFooter = {ATA0, file->sector, base, strlen(_FILE_FOOTER)+1,footer};

	// And read it
	System.readDisk(&fileHeader);
	System.readDisk(&fileName);
	System.readDisk(&fileSize);
	System.readDisk(&fileParent);
	System.readDisk(&fileFooter);

	// Its a file
	if(!strcmp(header,_FILE_HEADER) && !strcmp(footer,_FILE_FOOTER)){

		// Blabla, things to do


		return file;
	}


	// File not recognized
	__freeFile(__getFileIndexBySector(file->sector));
	return NULL;
}


// Allocate a file on disk!
void __allocateFile(File * file){
	// Set sector on sectorTable
	setSector(file->sector);

	// And set all necessary sectors for file
	int sectors = (file->size / 512) + 1;
	int i;
	for(i=0;i<sectors;i++)
		setSector(file->sector+i);

	// Update table on disk, not that expensive
	exportTable();

	// FILE HEADER
	// 255 bytes: name
	// 4 bytes: size
	// 4 bytes: parent sector
	// FILE FOOTER

	int base = 0;

	disk_cmd fileHeader = {ATA0, file->sector, base, strlen(_FILE_HEADER)+1,_FILE_HEADER};
	base += strlen(_FILE_HEADER)+1;

	// Now prepare file header on sector
	disk_cmd fileName = {ATA0, file->sector, base, _FILE_NAMELENGTH, file->name};
	base += _FILE_NAMELENGTH;

	disk_cmd fileSize = {ATA0, file->sector,base, sizeof(int), (char*)(&(file->size))};
	base += sizeof(int);

	int parentSector = file->parent != NULL ? file->parent->sector : -1;
	disk_cmd fileParent = {ATA0, file->sector, base, sizeof(int), (char *)(&parentSector)};
	base += sizeof(int);

	disk_cmd fileFooter = {ATA0, file->sector, base, strlen(_FILE_FOOTER)+1, _FILE_FOOTER };

	// And allocate it
	System.writeDisk(&fileHeader);
	System.writeDisk(&fileName);
	System.writeDisk(&fileSize);
	System.writeDisk(&fileParent);
	System.writeDisk(&fileFooter);
}


void __printFile(File * f){
	printf("Filename: %s\n", f->name);
	printf("Sector: %d\n", f->sector);
	printf("Size: %d\n", f->size);
	printf("Parent: %s\n", f->parent != NULL ? f->parent->name : "null");
}

void __initFileName(File * f){
	int i;
	for(i=0;i<_FILE_NAMELENGTH;i++)
		f->name[i] = 0;
}

File * __createFile(char * name, int sector, int size, File * parent){
	File * out = getFreeFile();

	// Prepare name
	__initFileName(out);
	_memcpy(name, out->name, strlen(name)+1);

	out->sector = sector;
	out->size = size;
	out->parent = parent;
	if(parent != NULL){
		out->psector = parent->sector;
		__addFileChild(parent,out);
	}
	else out->psector = -1;

	int i;
	for(i=0;i<_FILE_CHILDREN;i++)
		out->children[i] = NULL;
	
	return out;
}


File * __getFileBySector(int sector){
	if(sector == -1 )
		return NULL;

	int i;
	for(i=0;i<_MAX_FILES;i++)
		if(fileTable[i].sector == sector)
			return fileTable+i;
}


void __addFileChild(File * parent, File * child){
	int i;
	for(i=0;i<_FILE_CHILDREN;i++)
		if(parent->children[i] == NULL){
			parent->children[i] = child;
			return;
		}
}


void __constructFileTree(){

	int i;
	for(i=0;i<_MAX_FILES;i++)
		if(fileTable[i].sector != -1){

			File * f = fileTable+i;

			// Get parent and construct tree
			File * parent = __getFileBySector(f->psector);
			if(parent != NULL){

				// Add parent to child
				f->parent = parent;

				// Add child to parent
				__addFileChild(parent, f);

			}
		}
}


// Just load it, then do processing
void __loadFileTable(){
	int i;
	for(i=0;i<_MAX_FILES;i++)
		fileTable[i].sector = -1;
	
	for(i=_FILETABLE_SECTOR_START;i<_MAX_SECTORS;i++)
		if(getSector(i)){

			printf("Loading file from sector %d\n", i);
			// Try to load file
			File * file = __loadFile(i);
		}

	// Now process and construct tree
	__constructFileTree();


	__setCurrentDir(fileTable);
}

void __printFileTree(File * f){
	__printFileTreeTabs(f, 0);
}


void __printChildren(File *f){
	int i;
	for(i=0;i<_FILE_CHILDREN;i++)
		if(f->children[i]!=NULL)
			printf("%s\n", f->children[i]->name);

}

void __printFileTreeTabs( File *f, int tabs){
	int i;
	for(i=0;i<tabs;i++)
		if(i)
			printf(" |\t\t");
		else printf(" \t\t");

	if(tabs)
		printf(" |-");

	printf("%s",f->name);

	int hasChilds = 0;

	for(i=0;i<_FILE_CHILDREN;i++)
		if(f->children[i] != NULL){

			// Print it nicely
			if(!hasChilds){
				hasChilds = 1;
				printf("\n");
			}

			__printFileTreeTabs(f->children[i], tabs+1);
		}

	if(!hasChilds) {
		printf("\n");
	}

}


void __initFileTable(){
	int i;
	for(i=0;i<_MAX_FILES;i++)
		fileTable[i].sector = -1;

	File * base = __createFile(_FILE_ROOT,__getFileFreeSector(),_DEFAULT_FILESIZE, NULL);
	__allocateFile(base);


	File * usr = __createFile("usr",__getFileFreeSector(),_DEFAULT_FILESIZE, base);
	__allocateFile(usr);

	File * bin = __createFile("bin", __getFileFreeSector(), _DEFAULT_FILESIZE,base);
	__allocateFile(bin);

	File * dev = __createFile("dev", __getFileFreeSector(), _DEFAULT_FILESIZE,base);
	__allocateFile(dev);

	File * home = __createFile("home", __getFileFreeSector(), _DEFAULT_FILESIZE,base);
	__allocateFile(home);


	__setCurrentDir(base);
}

int __fileHasChildren(File * file){
	int i;
	for(i=0;i<_FILE_CHILDREN;i++)
		if(file->children[i] != NULL)
			return 0;

	return 1;
}


int fileIsDir(File * file){
	return __fileHasChildren(file);
}

File * __getFileByName(char * name, File * parent){
	if(parent == NULL)
		return NULL;

	if(!strcmp(parent->name,name))
		return parent;
	else {
		int i;
		for(i=0;i<_FILE_CHILDREN;i++)
			if(__getFileByName(name,parent->children[i]) != NULL)
				return parent->children[i];
	}
	return NULL;
}

File * getFileByName(char * name){
	File * root = fileTable;
	return __getFileByName(name,root);
}


File * __getChildByFileName( char * name, File * parent){
	int i;
	for(i=0;i<_FILE_CHILDREN;i++)
		if(parent->children[i] != NULL)
			if(!strcmp(parent->children[i]->name, name))
				return parent->children[i];
	return NULL;
}


int openFile( char * name, int flags){
	File * curr = __getCurrentDir();
	File * file = __getChildByFileName(name,curr);

	if(file == NULL){
		file = __createFile(name, __getFileFreeSector(), _DEFAULT_FILESIZE, __getCurrentDir());
		__allocateFile(file);
	} else return -1;

	// There is no file descriptor :P
	return 0;
}


int readFile(File * file, char * buff, int count){
	if(file == NULL)
		return -1;

	if( count > file->size )
		return -2;

	int sector = file->sector+1;
	disk_cmd fileData = {ATA0, sector, 0, count, buff};

	// And allocate it
	System.readDisk(&fileData);


	return 0;
}

int writeFile(File * file, char * buff, int count){

	if(file == NULL)
		return -1;

	if( count > file->size -1) // EOF place
		return -2;


	int sector = file->sector+1;
	int base = 0;

	disk_cmd fileData = {ATA0, sector, base, count, buff};
	base += count;

	char eof = EOF;

	disk_cmd fileEnd = {ATA0, sector, base, 1, &eof };

	// And allocate it
	System.writeDisk(&fileData);
	System.writeDisk(&fileEnd);
	
	return 0;
}

int vim(char * arg){





}


int cat(char * arg){
	File * file = getFileByName(arg);

	if(file == NULL) return -1;

	char * tmp = (char*) malloc(file->size);

	readFile(file, tmp, file->size);

	printf("File %s:\n", file->name);

	int i;
	for(i=0;i<file->size;i++)
		putchar(tmp[i]);


	printf("\nEnd of file \n");
	return 0;
}


int touch( char * arg ){
	openFile(arg, 0);

	return 0;
}

int tree(char *a){
	File * file = getFileByName(a);
	__printFileTree(file);
	return 0;
}

int ls(char * arg){


	printf("ls %s\n", arg);


	File * file = getFileByName(arg);

	if(file!=NULL){

		int i;
		for(i=0;i<_FILE_CHILDREN;i++)
			if(file->children[i] != NULL){
				printf("/%s\t", file->children[i]->name);
			}

		printf("\n");
		return 0;
	}

	return -1;
}

int fileIO(char * a){

	printf("Vamos a crear la tabla\n");
	printf("Que hago?\n1) crear tabla\n2) cargar tabla\n3)pasar\n");
	int opt = 0;
	scanf("%d",&opt);
	printf("\n");

	if(opt == 1)
		createFilesystem();
	else if(opt== 2)
		loadFileSystem();
	else if(opt == 3);

	File * f = fileTable;

	printf("\nFile tree: \n");
	ls("%");

//	if( openFile("hola", 0) == -1 )
//		printf("File already exists.\n");

	File * hola = getFileByName("hola");

	char * buf = "Hola como estas";

//	writeFile(hola, buf, strlen(buf));

//	printf("Sector de hola: %d\n", hola->sector);


	//cat("hola");

	tree("%");
}
