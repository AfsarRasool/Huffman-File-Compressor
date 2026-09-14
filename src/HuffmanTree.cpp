#include "HuffmanTree.h"

#include <iomanip>
#include <iostream>

void buildMinHeap(MinHeap &q, char letters[], int freq[], int size)
{
    for (int i = 0; i < size; i++)
    {
        if (freq[i] > 0)
        {
            Node *temp = new Node(letters[i], freq[i]);
            q.push(temp);
        }
    }
}

void printHeap(MinHeap q)
{
    while (!q.empty())
    {
        Node *temp = q.top();
        q.pop();
        std::cout << temp->letter << "  " << temp->freq << std::endl;
    }
}

Node *compress(MinHeap q)
{
    std::cout << "Compressing....!" << std::endl;

    if (q.empty())
        return nullptr;

    while (q.size() > 1)
    {
        Node *left = q.top();
        q.pop();

        Node *right = q.top();
        q.pop();

        int sumFreq = left->freq + right->freq;
        Node *temp = new Node('$', sumFreq);
        temp->left = left;
        temp->right = right;
        q.push(temp);
    }

    return q.top();
}

void levelOrderTraversal(Node *root)
{
    std::cout << "Level Order traversal...." << std::endl;

    if (root == nullptr)
    {
        std::cout << "Empty!" << std::endl;
        return;
    }

    std::queue<Node *> q;
    q.push(root);

    while (!q.empty())
    {
        int size = static_cast<int>(q.size());

        for (int i = 0; i < size; i++)
        {
            Node *temp = q.front();
            q.pop();

            std::cout << temp->letter << " " << temp->freq << ", ";

            if (temp->left)
                q.push(temp->left);

            if (temp->right)
                q.push(temp->right);
        }

        std::cout << std::endl;
    }

    std::cout << std::endl;
}

void assignCodes(Node *root, const std::string &code, StoreCodes codes[])
{
    if (root == nullptr)
        return;

    if (root->left == nullptr && root->right == nullptr)
    {
        codes[root->letter - 'a'].alphabet = root->letter;
        codes[root->letter - 'a'].code = code;
        return;
    }

    assignCodes(root->left, code + "0", codes);
    assignCodes(root->right, code + "1", codes);
}

void printCodes(Node *root, const std::string &code)
{
    if (root == nullptr)
        return;

    if (root->left == nullptr && root->right == nullptr)
    {
        std::cout << std::left << std::setw(12) << root->letter << "| " << code << std::endl;
        return;
    }

    printCodes(root->left, code + "0");
    printCodes(root->right, code + "1");
}

void clearPriorityQueue(MinHeap &q)
{
    while (!q.empty())
    {
        Node *temp = q.top();
        q.pop();
        delete temp;
    }
}

void deleteTree(Node *root)
{
    if (root == nullptr)
        return;

    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

void resetCodes(StoreCodes codes[], int size)
{
    for (int i = 0; i < size; i++)
    {
        codes[i].alphabet = '$';
        codes[i].code = "";
    }
}
