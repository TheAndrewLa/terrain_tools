#ifndef HUFFMANCOMPRESSOR_H
#define HUFFMANCOMPRESSOR_H
#include "../Types.h"
#include <cstring>

using namespace types;

namespace terraingenerator {
template <typename T>
class HuffmanCommpressor
{
public:
    struct Node {
        uchar symbol;
        T freq;
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
    uint32 codes[256];
    uchar codeLengths[256];

    void insert(Node** heap, uint32& size, Node* node) {
        heap[size++] = node;
        int i = size - 1;
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (heap[parent]->freq <= heap[i]->freq) {
                break;
            }
            Node* tmp = heap[parent];
            heap[parent] = heap[i];
            heap[i] = tmp;
            i = parent;
        }
    }

    Node* extractMin(Node** heap, uint32& size) {
        Node* minNode = heap[0];
        heap[0] = heap[--size];
        uint32 i = 0;
        while (true) {
            uint32 left = 2 * i + 1;
            uint32 right = 2 * i + 2;
            uint32 smallest = i;
            if (left < size && heap[left]->freq < heap[smallest]->freq) {
                smallest = left;
            }
            if (right < size && heap[right]->freq < heap[smallest]->freq) {
                smallest = right;
            }
            if (smallest == i) {
                break;
            }
            Node* tmp = heap[i];
            heap[i] = heap[smallest];
            heap[smallest] = tmp;
            i = smallest;
        }
        return minNode;
    }

    void buildCodeTable(Node* node, uint32 code, uchar length) {
        if (!node->left && !node->right) {
            codes[node->symbol] = code;
            codeLengths[node->symbol] = length;
            return;
        }
        if (node->left) {
            buildCodeTable(node->left,  (code << 1), length + 1);
        }
        if (node->right) {
            buildCodeTable(node->right, (code << 1) | 1, length + 1);
        }
    }

public:
    HuffmanCommpressor(T freq[256]) {
        memset(codes, 0, sizeof(codes));
        memset(codeLengths, 0, sizeof(codeLengths));
        Node* heap[256];
        uint32 heapSize = 0;
        for (uchar i = 0; i <= 255; ++i) {
            if (freq[i] > 0) {
                insert(heap, heapSize, new Node(i, freq[i]));
            }
            if (i == 255) {
                break;
            }
        }
        while (heapSize > 1) {
            Node* a = extractMin(heap, heapSize);
            Node* b = extractMin(heap, heapSize);
            insert(heap, heapSize, new Node(0, a->freq + b->freq, a, b));
        }
        root = heapSize == 1 ? heap[0] : nullptr;
        buildCodeTable(root, 0, 0);
    }

    uchar* compress(const uchar* data, uint32 dataSize, uint32& outSize, uchar firstValue) {
        uchar* out = new uchar[dataSize];
        int bitPos = 0, bytePos = 0;
        out[0] = 0;
        for (int i = 1; i < dataSize; ++i) {
            uint32 code = codes[data[i]];
            int len = codeLengths[data[i]];
            for (int b = len - 1; b >= 0; --b) {
                if ((code >> b) & 1) {
                    out[bytePos] |= (1 << (7 - bitPos));
                }
                bitPos++;
                if (bitPos == 8) {
                    bitPos = 0;
                    out[++bytePos] = 0;
                }
            }
        }
        outSize = bytePos + (bitPos > 0 ? 1 : 0);
        return out;
    }

    uchar* decompress(const uchar* data, uint32 dataSize, uint32 expectedSize, uchar firstValue) {
        uchar* out = new uchar[expectedSize];
        uint32 outIndex = 0;
        uint32 bytePos = 0, bitPos = 0;
        Node* node = root;
        uchar current = firstValue;
        out[outIndex++] = current;
        while (bytePos < dataSize && outIndex < expectedSize) {
            int bit = (data[bytePos] >> (7 - bitPos)) & 1;
            bitPos++;
            if (bitPos == 8) {
                bitPos = 0;
                bytePos++;
            }
            node = bit ? node->right : node->left;
            if (node->left == nullptr && node->right == nullptr) {
                current += static_cast<char>(node->symbol);
                out[outIndex++] = current;
                node = root;
            }
        }
        return out;
    }

    ~HuffmanCommpressor() {
        delete root;
    }
};
}

#endif // HUFFMANCOMPRESSOR_H
