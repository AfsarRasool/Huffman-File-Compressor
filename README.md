<<<<<<< HEAD
# Huffman File Compressor

A C++ file compression and decompression project based on the Huffman Coding algorithm.

## Project Structure

```text
Huffman-File-Compressor/
├── include/
│   ├── HuffmanTypes.h
│   ├── HuffmanTree.h
│   └── FileHandler.h
├── src/
│   ├── HuffmanTree.cpp
│   ├── FileHandler.cpp
│   └── main.cpp
└── README.md
```

## Build

```bash
g++ -std=c++11 -Iinclude src/main.cpp src/HuffmanTree.cpp src/FileHandler.cpp -o huffman
```

## Run

Linux/macOS:

```bash
./huffman
```

Windows:

```bash
huffman.exe
```

## Main Components

- `HuffmanTypes.h` — Huffman tree node, priority-queue comparator, and code storage type.
- `HuffmanTree.cpp` — heap construction, Huffman tree construction, code generation, traversal, and cleanup.
- `FileHandler.cpp` — frequency counting, binary header/table handling, bitstream encoding/decoding, and summaries.
- `main.cpp` — command-line menu and program flow.
=======
# Huffman-File-Compressor
A C++ file compression and decompression program using Huffman Coding.
>>>>>>> 9771f539284f28149e87bbea403087d0409be499
