# Compiler
CC = g++

# Compiler flags
CFLAGS = -c

# Linker flags
LDFLAGS = -lcurses

# Source files
SRCS = main.cpp backend.cpp block.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable name
EXECUTABLE = tetris

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(EXECUTABLE)

# Compile source files into object files
%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJS) $(EXECUTABLE)
