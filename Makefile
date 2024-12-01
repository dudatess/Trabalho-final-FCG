# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wno-unused-function -g -I./include/

# Directories
SRCDIR = src
INCDIR = include
BINDIR = bin/Linux

# Libraries
LIBS = ./lib-linux/libglfw3.a -lrt -lm -ldl -lX11 -lpthread -lXrandr -lXinerama -lXxf86vm -lXcursor

# Sources and objects
SOURCES = $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(BINDIR)/%.o, $(SOURCES))
OBJECTS := $(patsubst $(SRCDIR)/%.c, $(BINDIR)/%.o, $(OBJECTS))

# Target executable
TARGET = $(BINDIR)/main

# Default target
all: $(TARGET)

# Create binary directory
$(BINDIR):
	mkdir -p $(BINDIR)

# Link the executable
$(TARGET): $(BINDIR) $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) $(LIBS)

# Compile .cpp files
$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile .c files
$(BINDIR)/%.o: $(SRCDIR)/%.c
	gcc -I$(INCDIR) -c $< -o $@

.PHONY: clean run

clean:
	rm -rf $(BINDIR)

run: $(TARGET)
	cd $(BINDIR) && ./main
