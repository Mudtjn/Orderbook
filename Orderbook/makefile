# Compiler and flags
CXX = c++
CXXFLAGS = -O3 -Wall -shared -std=c++20 -fPIC
PYBIND11_INCLUDES = $(shell python3 -m pybind11 --includes)
EXT_SUFFIX = $(shell python3-config --extension-suffix)

# Target name
TARGET = orderbook$(EXT_SUFFIX)

# Source files
SOURCES = Side.cpp \
          OrderTypes.cpp \
          BindOrder.cpp \
          BindOrderModify.cpp \
          BindOrderbook.cpp \
          BindOrderbookLevelInfos.cpp \
          BindLevelInfo.cpp \
          BindOrderTypes.cpp \
          BindSide.cpp \
					BindTrades.cpp \
          Orderbook.cpp

# Object files (optional, for incremental builds)
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

# Build the shared library
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(PYBIND11_INCLUDES) $^ -o $@

# Clean build artifacts
clean:
	rm -f $(TARGET) *.o

# Install to Python site-packages (optional)
install: $(TARGET)
	pip install -e .

# Run tests (optional)
test: $(TARGET)
	python3 -m pytest tests/

# Phony targets
.PHONY: all clean install test
