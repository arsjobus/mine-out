# Compiler and flags
CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Homebrew prefix (auto-detect for Apple Silicon vs Intel)
/usr/local/bin/brew: BREW_PREFIX = /usr/local
/opt/homebrew/bin/brew: BREW_PREFIX = /opt/homebrew
BREW_PREFIX ?= $(shell brew --prefix)

# SFML path from Homebrew
SFML_PREFIX = $(shell brew --prefix sfml)
INCLUDES = -I$(SFML_PREFIX)/include
LIB_PATH = -L$(SFML_PREFIX)/lib
LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# macOS required frameworks for SFML
FRAMEWORKS = -framework Foundation -framework Cocoa -framework IOKit -framework CoreVideo

# Source and object files
SRC_DIR = src
BUILD_DIR = build
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(SRC_FILES:.cpp=.o)

# Output binary path
TARGET = $(BUILD_DIR)/game

# Default build target
all: $(TARGET) sign remove_quarantine

# Create build dir and link all object files into final executable
$(TARGET): $(OBJ_FILES)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIB_PATH) -o $@ $^ $(LIBS) $(FRAMEWORKS) \
		-Wl,-rpath,$(SFML_PREFIX)/lib \
		-Wl,-rpath,$(BREW_PREFIX)/opt/freetype/lib \
		-Wl,-rpath,$(BREW_PREFIX)/opt/libvorbisenc/lib

# Compile .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Sign executable and Homebrew SFML dylibs
sign:
	@echo "Signing executable and SFML dylibs from Homebrew..."
	$(CODESIGN) $(TARGET)
	@for dylib in $(SFML_PREFIX)/lib/libsfml-*.dylib; do \
		$(CODESIGN) "$$dylib"; \
	done

# Fallback codesign tool
CODESIGN = codesign --force --sign -

# Clean object files and the built binary
clean:
	rm -f $(OBJ_FILES) $(TARGET)

