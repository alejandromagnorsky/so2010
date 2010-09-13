##
##  SISTEMAS OPERATIVOS, 2C 2010
##  PRIMER TRABAJO PRACTICO
##  Simulacion de una colonia de hormigas.
##
##  Makefile.
##  
##

CC := gcc
CFLAGS := -c -g
LDFLAGS := -lpthread -lncurses

SOURCEDIR := src
HEADERDIR := include

SOURCES := $(wildcard $(SOURCEDIR)/*.c)
#SRCNAMES := /main /ipc /ipc_sock /ipc_queue /ipc_fifo /ipc_shm /queue /message /tools /ant /cmd
#SOURCES := $(SRCNAMES:/%=$(SOURCEDIR)/%.c)
HEADERS := $(SRCNAMES:/%=$(SOURCEDIR)/%.h)
OBJECTS := $(SOURCES:%.c=%.o)


all: $(OBJECTS) anthill

anthill: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
    
.o:
	@echo $(@:%.o=$(HEADERDIR)/%.h)
	$(CC) $(CFLAGS) $< -o $@

rebuild: clean all
	
clean:
	rm -f anthill
	rm -f $(SOURCEDIR)/*.o
