# Compiler
CC = g++

# Compiler flags
CFLAGS = -c

# Linker flags
LDFLAGS = -lncurses

# Source files
SOURCES = tetris.cpp main.cpp game.cpp graphics.cpp
OBJECTS = $(SOURCES:.cpp=.o)

# Executable name
EXECUTABLE = tetris

# Header file
HEADER = tetris.h

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)
