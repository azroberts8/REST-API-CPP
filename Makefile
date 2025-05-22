# Compiler and flags
CXX := g++
CXX_FLAGS := -Wall -Wextra -I./src

# Source files
SOURCE_DIR := src
BUILD_DIR := build
SOURCES := $(SOURCE_DIR)/main.cpp $(SOURCE_DIR)/http_tcpServer.cpp
OBJECTS := $(patsubst $(SOURCE_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))

# Executable
TARGET := $(BUILD_DIR)/main


build: $(TARGET)

# Linking
$(TARGET): $(OBJECTS)
	$(CXX) $(CXX_FLAGS) -o $@ $^

# Compilation
$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXX_FLAGS) -c $< -o $@

run: build
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)