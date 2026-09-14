CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -Iinclude
TARGET = huffman
SOURCES = src/main.cpp src/HuffmanTree.cpp src/FileHandler.cpp

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET) *.o encoded.bin decoded.txt

.PHONY: all clean
