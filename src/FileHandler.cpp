#include "FileHandler.h"

#include <fstream>
#include <iomanip>
#include <iostream>

long long getFileSize(const std::string &filename)
{
    std::ifstream in(filename, std::ios::binary | std::ios::ate);
    if (!in)
        return 0;

    return static_cast<long long>(in.tellg());
}

int countUniqueCharacters(const int freq[], int size)
{
    int count = 0;

    for (int i = 0; i < size; i++)
    {
        if (freq[i] > 0)
            count++;
    }

    return count;
}

void printCompressionSummary(const std::string &inputFileName,
                             const std::string &outputFileName,
                             long long originalSize,
                             long long compressedSize)
{
    std::cout << "\n=== Compression Summary ===" << std::endl;
    std::cout << "Input file          : " << inputFileName << std::endl;
    std::cout << "Original size       : " << originalSize << " bytes" << std::endl;
    std::cout << "Compressed file     : " << outputFileName << std::endl;
    std::cout << "Compressed size     : " << compressedSize << " bytes" << std::endl;

    if (originalSize > 0)
    {
        double reducedPercent =
            (static_cast<double>(originalSize - compressedSize) / originalSize) * 100.0;

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Size reduced by     : " << reducedPercent << "%" << std::endl;
    }
    else
    {
        std::cout << "Size reduced by     : 0.00%" << std::endl;
    }
}

void printDecompressionSummary(const std::string &decodedFileName,
                               const char letters[],
                               const int freq[],
                               int size)
{
    std::cout << "\n=== Decompression Summary ===" << std::endl;
    std::cout << "Unique characters   : ";

    bool first = true;
    for (int i = 0; i < size; i++)
    {
        if (freq[i] > 0)
        {
            if (!first)
                std::cout << ", ";

            std::cout << letters[i];
            first = false;
        }
    }

    std::cout << std::endl;

    long long decodedSize = getFileSize(decodedFileName);
    std::cout << "Decoded file        : " << decodedFileName << std::endl;
    std::cout << "File size after decompression : " << decodedSize << " bytes" << std::endl;
}

void writeTable(const char letters[], const int freq[], int n,
                const std::string &outputFilename)
{
    std::ofstream out(outputFilename, std::ios::binary);
    if (!out)
        return;

    int count = 0;
    for (int i = 0; i < n; i++)
    {
        if (freq[i] > 0)
            count++;
    }

    out.write(reinterpret_cast<const char *>(&count), sizeof(count));

    for (int i = 0; i < n; i++)
    {
        if (freq[i] == 0)
            continue;

        out.write(&letters[i], 1);
        out.write(reinterpret_cast<const char *>(&freq[i]), sizeof(int));
    }

    long long totalBitsPlaceholder = 0;
    out.write(reinterpret_cast<const char *>(&totalBitsPlaceholder),
              sizeof(totalBitsPlaceholder));
}

void readTableFromFile(int freq[], int n, const std::string &filename)
{
    std::cout << "Reading from file....." << std::endl;

    std::ifstream in(filename, std::ios::binary);
    if (!in)
        return;

    for (int i = 0; i < n; i++)
        freq[i] = 0;

    int count = 0;
    in.read(reinterpret_cast<char *>(&count), sizeof(count));

    for (int k = 0; k < count; k++)
    {
        char ch;
        int fr;

        in.read(&ch, 1);
        std::cout << ch << " ";
        in.read(reinterpret_cast<char *>(&fr), sizeof(int));

        freq[ch - 'a'] = fr;
    }

    std::cout << std::endl;
}

void writeBitStream(const std::string &inputFileName,
                    const std::string &outputFileName,
                    StoreCodes codes[])
{
    std::ifstream in(inputFileName);
    std::ofstream out(outputFileName, std::ios::binary | std::ios::app);

    if (!in || !out)
    {
        std::cout << "File not opened for bitstream" << std::endl;
        return;
    }

    unsigned char buffer = 0;
    int bitCount = 0;
    long long totalBits = 0;

    char ch;
    while (in.get(ch))
    {
        // Keep encoding consistent with readFileCountFreq():
        // uppercase letters become lowercase and non-letters are ignored.
        if (ch >= 'A' && ch <= 'Z')
            ch = static_cast<char>(ch - 'A' + 'a');

        if (ch < 'a' || ch > 'z')
            continue;

        std::string code = codes[ch - 'a'].code;

        for (char bit : code)
        {
            buffer <<= 1;
            buffer |= (bit == '1');
            bitCount++;
            totalBits++;

            if (bitCount == 8)
            {
                out.put(static_cast<char>(buffer));
                buffer = 0;
                bitCount = 0;
            }
        }
    }

    if (bitCount > 0)
    {
        buffer <<= (8 - bitCount);
        out.put(static_cast<char>(buffer));
    }

    in.close();
    out.close();

    std::ifstream headerIn(outputFileName, std::ios::binary);
    if (!headerIn)
        return;

    int count = 0;
    headerIn.read(reinterpret_cast<char *>(&count), sizeof(count));
    headerIn.close();

    std::streamoff placeholderOffset =
        sizeof(count) + static_cast<std::streamoff>(count) *
                            (1 + static_cast<std::streamoff>(sizeof(int)));

    std::fstream headerOut(outputFileName,
                           std::ios::binary | std::ios::in | std::ios::out);
    if (!headerOut)
        return;

    headerOut.seekp(placeholderOffset, std::ios::beg);
    headerOut.write(reinterpret_cast<const char *>(&totalBits), sizeof(totalBits));
}

void decodeBitstream(const std::string &inputFileName,
                     const std::string &outputFileName,
                     Node *root)
{
    std::ifstream in(inputFileName, std::ios::binary);
    std::ofstream out(outputFileName);

    if (!in || !out || root == nullptr)
    {
        std::cout << "File not opened in decodeBitstream" << std::endl;
        return;
    }

    int count = 0;
    in.read(reinterpret_cast<char *>(&count), sizeof(int));

    char ch;
    int fr;
    for (int i = 0; i < count; i++)
    {
        in.read(&ch, 1);
        in.read(reinterpret_cast<char *>(&fr), sizeof(int));
    }

    long long totalBits = 0;
    long long processedBits = 0;
    bool done = false;
    in.read(reinterpret_cast<char *>(&totalBits), sizeof(totalBits));

    unsigned char byte;
    Node *current = root;

    while (in.read(reinterpret_cast<char *>(&byte), 1) && !done)
    {
        for (int i = 7; i >= 0; i--)
        {
            int bit = (byte >> i) & 1;

            if (bit == 0)
                current = current->left;
            else
                current = current->right;

            processedBits++;

            if (current->left == nullptr && current->right == nullptr)
            {
                out.put(current->letter);
                current = root;
            }

            if (processedBits >= totalBits)
            {
                done = true;
                break;
            }
        }
    }
}

void readFileCountFreq(int freq[], const std::string &filename)
{
    std::ifstream in(filename);
    if (!in)
        return;

    for (int i = 0; i < 26; i++)
        freq[i] = 0;

    char ch;
    while (in.get(ch))
    {
        if (ch >= 'A' && ch <= 'Z')
            ch = static_cast<char>(ch - 'A' + 'a');

        if (ch >= 'a' && ch <= 'z')
            freq[ch - 'a']++;
    }
}

void printFreq(char letters[], int freq[], int size)
{
    for (int i = 0; i < size; i++)
    {
        if (freq[i] > 0)
            std::cout << letters[i] << "  " << freq[i] << std::endl;
    }
}
