#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "HuffmanTypes.h"
#include <string>

long long getFileSize(const std::string &filename);
int countUniqueCharacters(const int freq[], int size);

void printCompressionSummary(const std::string &inputFileName,
                             const std::string &outputFileName,
                             long long originalSize,
                             long long compressedSize);

void printDecompressionSummary(const std::string &decodedFileName,
                               const char letters[],
                               const int freq[],
                               int size);

void writeTable(const char letters[], const int freq[], int n,
                const std::string &outputFilename);

void readTableFromFile(int freq[], int n, const std::string &filename);

void writeBitStream(const std::string &inputFileName,
                    const std::string &outputFileName,
                    StoreCodes codes[]);

void decodeBitstream(const std::string &inputFileName,
                     const std::string &outputFileName,
                     Node *root);

void readFileCountFreq(int freq[], const std::string &filename);
void printFreq(char letters[], int freq[], int size);

#endif
