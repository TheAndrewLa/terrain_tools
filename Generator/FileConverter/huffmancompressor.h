#ifndef HUFFMANCOMPRESSOR_H
#define HUFFMANCOMPRESSOR_H
#include "../Types.h"
#include <cstring>

using namespace types;

namespace terraingenerator {
class HuffmanCommpressor
{
public:
    struct Node {
        uchar symbol;
        int freq;
        Node* left = nullptr;
        Node* right = nullptr;

        ~Node() {
            if (left != nullptr) {
                delete left;
            }
            if (right != nullptr) {
                delete right;
            }
        }
    };
private:
    Node* root = nullptr;
    unsigned int codes[256];
    uchar codeLengths[256];

    void insert(Node** heap, int& size, Node* node);

    Node* extractMin(Node** heap, int& size);

    void buildCodeTable(Node* node, unsigned int code, uchar length);

public:
    HuffmanCommpressor(int freq[256]);

    uchar* compress(const uchar* data, int dataSize, int& outSize, uchar firstValue);

    uchar* decompress(const uchar* data, int dataSize, int expectedSize, uchar firstValue);

    ~HuffmanCommpressor();
};
}

#endif // HUFFMANCOMPRESSOR_H
