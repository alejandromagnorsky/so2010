##
##  SISTEMAS OPERATIVOS, 2C 2010
##  PRIMER TRABAJO PRACTICO
##  Simulacion de una colonia de hormigas.
##
##  Makefile.
##  
##

CC := gcc
CFLAGS := -c
LDFLAGS := -lpthread

SOURCEDIR := src
HEADERDIR := include

#SOURCES := $(wildcard $(SOURCEDIR)/*.c)
SRCNAMES := /main /ipc /ipc_sock /ipc_queue /queue /message /tools
SOURCES := $(SRCNAMES:/%=$(SOURCEDIR)/%.c)
HEADERS := $(SRCNAMES:/%=$(SOURCEDIR)/%.h)
OBJECTS := $(SOURCES:%.c=%.o)


all: $(OBJECTS) anthill

anthill: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
    
.o:
	@echo $(@:%.o=$(HEADERDIR)/%.h)
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	rm $(SOURCEDIR)/*.o
