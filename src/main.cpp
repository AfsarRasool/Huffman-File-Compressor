#include "FileHandler.h"
#include "HuffmanTree.h"

#include <iomanip>
#include <iostream>
#include <string>

int main()
{
    MinHeap encodingQueue;
    MinHeap decodingQueue;

    char letters[] = {
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
        'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

    StoreCodes codes[26];
    int frequencies[26] = {0};
    int size = sizeof(frequencies) / sizeof(frequencies[0]);
    int choice;
    Node *compressedRoot = nullptr;

    do
    {
        std::cout << "\n1. Compress  2. Decompress  3. Quit" << std::endl;
        std::cin >> choice;

        if (choice == 3)
            break;

        std::string filename;

        switch (choice)
        {
        case 1:
        {
            std::cout << "Enter file name (abc.txt): ";
            std::cin >> filename;

            clearPriorityQueue(encodingQueue);

            if (compressedRoot != nullptr)
            {
                deleteTree(compressedRoot);
                compressedRoot = nullptr;
            }

            resetCodes(codes, size);
            readFileCountFreq(frequencies, filename);
            buildMinHeap(encodingQueue, letters, frequencies, size);
            compressedRoot = compress(encodingQueue);

            if (compressedRoot == nullptr)
            {
                std::cout << "No valid characters found in the input file." << std::endl;
                break;
            }

            assignCodes(compressedRoot, "", codes);

            std::cout << "\n=== Huffman Character Codes ===" << std::endl;
            std::cout << std::left << std::setw(12) << "Character" << "| Code" << std::endl;
            std::cout << "--------------------------------" << std::endl;
            printCodes(compressedRoot, "");

            const std::string outputFile = "encoded.bin";
            writeTable(letters, frequencies, size, outputFile);
            writeBitStream(filename, outputFile, codes);

            long long originalSize = getFileSize(filename);
            long long compressedSize = getFileSize(outputFile);
            printCompressionSummary(filename, outputFile, originalSize, compressedSize);
            break;
        }

        case 2:
        {
            std::cout << "Enter file name (abc.bin): ";
            std::cin >> filename;

            clearPriorityQueue(decodingQueue);

            if (compressedRoot != nullptr)
            {
                deleteTree(compressedRoot);
                compressedRoot = nullptr;
            }

            resetCodes(codes, size);
            readTableFromFile(frequencies, size, filename);
            buildMinHeap(decodingQueue, letters, frequencies, size);
            compressedRoot = compress(decodingQueue);

            if (compressedRoot == nullptr)
            {
                std::cout << "Unable to build Huffman tree from the compressed file." << std::endl;
                break;
            }

            std::cout << "\n=== Huffman Character Codes ===" << std::endl;
            std::cout << std::left << std::setw(12) << "Character" << "| Code" << std::endl;
            std::cout << "--------------------------------" << std::endl;
            printCodes(compressedRoot, "");

            const std::string decodedFile = "decoded.txt";
            decodeBitstream(filename, decodedFile, compressedRoot);
            printDecompressionSummary(decodedFile, letters, frequencies, size);
            break;
        }

        default:
            std::cout << "Invalid choice." << std::endl;
            break;
        }

    } while (true);

    if (compressedRoot != nullptr)
        deleteTree(compressedRoot);

    return 0;
}
