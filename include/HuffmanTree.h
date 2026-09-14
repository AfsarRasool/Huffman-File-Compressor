#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include "HuffmanTypes.h"
#include <queue>
#include <string>
#include <vector>

using MinHeap = std::priority_queue<Node *, std::vector<Node *>, Compare>;

void buildMinHeap(MinHeap &q, char letters[], int freq[], int size);
void printHeap(MinHeap q);
Node *compress(MinHeap q);
void levelOrderTraversal(Node *root);
void assignCodes(Node *root, const std::string &code, StoreCodes codes[]);
void printCodes(Node *root, const std::string &code);
void clearPriorityQueue(MinHeap &q);
void deleteTree(Node *root);
void resetCodes(StoreCodes codes[], int size);

#endif
