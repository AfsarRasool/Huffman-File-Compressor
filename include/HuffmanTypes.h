#ifndef HUFFMAN_TYPES_H
#define HUFFMAN_TYPES_H

#include <string>

class Node
{
public:
    char letter;
    int freq;
    Node *left;
    Node *right;

    Node(char a, int frequency)
        : letter(a), freq(frequency), left(nullptr), right(nullptr) {}
};

class Compare
{
public:
    bool operator()(Node *a, Node *b) const
    {
        return a->freq > b->freq;
    }
};

class StoreCodes
{
public:
    char alphabet;
    std::string code;

    StoreCodes() : alphabet('$'), code("") {}
};

#endif
