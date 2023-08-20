# Compiler
CC = g++

# Compiler flags
CFLAGS = -c

# Linker flags
LDFLAGS = -lcurses

all: main


.PHONY:
clean: 
	rm -f main
