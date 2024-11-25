# Compiler and Flags
CXX = g++
CXXFLAGS_DEBUG = -g -Wall -std=c++17 -Iinclude
CXXFLAGS_RELEASE = -O3 -Wall -std=c++17 -Iinclude

# Directory Structure
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
DEBUG_DIR = $(BUILD_DIR)/debug
RELEASE_DIR = $(BUILD_DIR)/release

# Source and Header Files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
HEADERS = $(wildcard $(INCLUDE_DIR)/*.h)

# Object Files
OBJECTS_DEBUG = $(patsubst $(SRC_DIR)/%.cpp, $(DEBUG_DIR)/%.o, $(SOURCES))
OBJECTS_RELEASE = $(patsubst $(SRC_DIR)/%.cpp, $(RELEASE_DIR)/%.o, $(SOURCES))

# Binaries
EXECUTABLE_DEBUG = pubman_debug.exe
EXECUTABLE_RELEASE = pubman_release.exe

# Default Target
all: $(EXECUTABLE_DEBUG) $(EXECUTABLE_RELEASE)

# Debug Binary
$(EXECUTABLE_DEBUG): $(OBJECTS_DEBUG)
	$(CXX) $(CXXFLAGS_DEBUG) $^ -o $@

# Release Binary
$(EXECUTABLE_RELEASE): $(OBJECTS_RELEASE)
	$(CXX) $(CXXFLAGS_RELEASE) $^ -o $@

# Debug Object Files
$(DEBUG_DIR)/%.o: $(SRC_DIR)/%.cpp
	@if not exist "$(DEBUG_DIR)" mkdir "$(DEBUG_DIR)"
	$(CXX) $(CXXFLAGS_DEBUG) -c $< -o $@

# Release Object Files
$(RELEASE_DIR)/%.o: $(SRC_DIR)/%.cpp
	@if not exist "$(RELEASE_DIR)" mkdir "$(RELEASE_DIR)"
	$(CXX) $(CXXFLAGS_RELEASE) -c $< -o $@

# Clean Target
clean:
	@if exist "$(BUILD_DIR)" rmdir /s /q "$(BUILD_DIR)"
	@if exist $(EXECUTABLE_DEBUG) del $(EXECUTABLE_DEBUG)
	@if exist $(EXECUTABLE_RELEASE) del $(EXECUTABLE_RELEASE)

# Phony Targets
.PHONY: all clean


