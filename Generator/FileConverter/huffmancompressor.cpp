#include "huffmancompressor.h"

using namespace terraingenerator;

void HuffmanCommpressor::insert(Node** heap, int& size, Node* node) {
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

HuffmanCommpressor::Node* HuffmanCommpressor::extractMin(Node** heap, int& size) {
    Node* minNode = heap[0];
    heap[0] = heap[--size];
    int i = 0;
    while (true) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;
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

void HuffmanCommpressor::buildCodeTable(Node* node, unsigned int code, uchar length) {
    if (!node->left && !node->right) {
        codes[node->symbol] = code;
        codeLengths[node->symbol] = length;
        return;
    }
    if (node->left) {
        buildCodeTable(node->left,  (code << 1),     length + 1);
    }
    if (node->right) {
        buildCodeTable(node->right, (code << 1) | 1, length + 1);
    }
}

HuffmanCommpressor::HuffmanCommpressor(int freq[256]) : root(nullptr) {
    memset(codes, 0, sizeof(codes));
    memset(codeLengths, 0, sizeof(codeLengths));
    Node* heap[256];
    int heapSize = 0;
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

uchar* HuffmanCommpressor::compress(const uchar* data, int dataSize, int& outSize, uchar firstValue) {
    uchar* out = new uchar[dataSize];
    int bitPos = 0, bytePos = 0;
    out[0] = 0;
    for (int i = 1; i < dataSize; ++i) {
        unsigned int code = codes[data[i]];
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

uchar* HuffmanCommpressor::decompress(const uchar* data, int dataSize, int expectedSize, uchar firstValue) {
    uchar* out = new uchar[expectedSize];
    int outIndex = 0;
    int bytePos = 0, bitPos = 0;
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
            current += (char)node->symbol;
            out[outIndex++] = current;
            node = root;
        }
    }
    return out;
}
HuffmanCommpressor::~HuffmanCommpressor() {
    delete root;
}
