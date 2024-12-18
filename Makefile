workspaceFolder = .
classesFolder = ./Classes
includeFolder = ./include
binFolder = ./bin

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++17 -I${classesFolder} -I${includeFolder} -DGLM_ENABLE_EXPERIMENTAL `pkg-config --cflags opencv4`

# Linker flags (for linking OpenCV libraries)
LDFLAGS = `pkg-config --libs opencv4`

# Source and object files
SRC_FILES = $(wildcard ${classesFolder}/*.cpp)
OBJ_FILES = $(patsubst ${classesFolder}/%.cpp, ${binFolder}/%.o, $(SRC_FILES))

# Output executable
TARGET = main

# Default rule to build the target
all: $(TARGET)

# Rule to link the object files and create the executable
$(TARGET): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Rule to compile source files into object files
$(binFolder)/%.o: $(classesFolder)/%.cpp
	mkdir -p $(binFolder)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(OBJ_FILES) $(TARGET)

# Combined clean and build target
rebuild: clean all

# Phony targets
.PHONY: all clean rebuild
