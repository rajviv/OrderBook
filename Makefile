# Compiler settings
CXX := g++
CXXFLAGS := -std=c++17
INCLUDE := -I ./include

# Directories
SRC_DIR := ./src
BUILD_DIR := ./build

# Source files
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))

# Target
TARGET := orderBook

# Build rule for objects
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

# Build rule for the target
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

all: $(TARGET)

.PHONY: clean

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
