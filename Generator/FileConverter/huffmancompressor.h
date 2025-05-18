#ifndef HUFFMANCOMPRESSOR_H
#define HUFFMANCOMPRESSOR_H
#include "../Types.h"
#include <cstring>
#include <cmath>
using namespace types;

namespace terraingenerator {
template <std::unsigned_integral T, std::floating_point U>
class HuffmanCommpressor
{
public:
    struct Node {
        T symbol;
        U freq;
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
    static constexpr uint32 count_T = std::numeric_limits<T>::max() + 1;

    Node* root = nullptr;

    uint32 codes[count_T];
    T codeLengths[count_T];

    void insert(Node** heap, uint32& size, Node* node) {
        heap[size++] = node;
        long long i = size - 1;
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

    void buildCodeTable(Node* node, uint32 code, T length) {
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
    HuffmanCommpressor(uint32 freq[count_T]) {
        memset(codes, 0, sizeof(codes));
        memset(codeLengths, 0, sizeof(codeLengths));
        Node* heap[count_T];
        uint32 heapSize = 0;
        for (uint32 i = 0; i < count_T; ++i) {
            if (freq[i] > 0) {
                insert(heap, heapSize, new Node(i, freq[i]));
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

    T* compress(const T* data, uint32 dataSize, uint32& outSize, T firstValue) {
        T* out = new T[dataSize];
        uint32 bitPos = 0, bytePos = 0;
        out[0] = 0;
        for (uint32 i = 1; i < dataSize; ++i) {
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

    T* decompress(const T* data, uint32 dataSize, uint32 expectedSize, T firstValue) {
        T* out = new T[expectedSize];
        uint32 outIndex = 0;
        uint32 bytePos = 0, bitPos = 0;
        Node* node = root;
        T current = firstValue;
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
