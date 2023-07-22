# SFML Makefile for BlueSimulator

# Compiler and compilation flags
CC = g++
CFLAGS = -std=c++14 -Wall -Wextra -g

# SFML
SFML_INCLUDE = -IC:/SFML/include
SFML_LIB = -LC:/SFML/lib -lsfml-graphics-d -lsfml-window-d -lsfml-system-d

# Directories
SRC_DIR = D:/BlueSimulator/src
OBJ_DIR = D:/BlueSimulator/obj
TARGET = BlueSimulator

# List of source files (add more as needed)
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)

# Generate list of corresponding object files in the obj directory
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

# Default target, which builds the executable
all: $(TARGET)

# Rule to generate object files from source files in the obj directory
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(SFML_INCLUDE)

# Rule to build the executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(SFML_LIB)

# Clean up the generated files
clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)

# Phony target to prevent conflicts with filenames
.PHONY: all clean