#include "fileconverter.h"

#include "../FileConverter/huffmancompressor.h"
#include <fstream>
#include <ios>
#include <iostream>

using namespace terraingenerator;

void FileConverter::to_file(Terrain &terrain, const char *file_name)
{
    int size = terrain.height_ * terrain.width_;
    uchar* delta = new uchar[size];

    delta[0] = terrain.heightmap_[0];
    for (int i = 1; i < size; ++i) {
        delta[i] = terrain.heightmap_[i] - terrain.heightmap_[i - 1];
    }

    int freq[256] = {0};
    for (int i = 1; i < size; ++i) {
        freq[delta[i]]++;
    }

    HuffmanCommpressor commpressor(freq);

    int compressedSize;
    uchar* compressed = commpressor.compress(delta, size, compressedSize, delta[0]);

    std::ofstream output_file(file_name, std::ios::binary);
    output_file.write((char*)&terrain.height_, sizeof(int));
    output_file.write((char*)&terrain.width_, sizeof(int));
    output_file.write((char*)freq, 256 * sizeof(int));
    output_file.put((char)delta[0]);
    output_file.write((char*)compressed, compressedSize);
    output_file.close();

    delete[] compressed;
    delete[] delta;
}

Terrain FileConverter::from_file(const char *file_name)
{
    std::ifstream input_file(file_name, std::ios::binary);

    int height, width;
    input_file.read((char*)&height, sizeof(int));
    input_file.read((char*)&width, sizeof(int));

    int freq[256];
    input_file.read((char*)freq, 256 * sizeof(int));

    uchar first;
    input_file.read((char*)&first, 1);

    int size = height * width;
    HuffmanCommpressor commpressor(freq);

    input_file.seekg(0, std::ios::end);
    int total_size = (int)input_file.tellg();
    int meta_size = sizeof(int) * 2 + sizeof(int) * 256 + 1;
    input_file.seekg(meta_size, std::ios::beg);

    int compressed_size = total_size - meta_size;
    uchar* compressed = new uchar[compressed_size];

    input_file.read((char*)compressed, compressed_size);
    input_file.close();

    uchar* heightmap = commpressor.decompress(compressed, compressed_size, size, first);

    delete[] compressed;
    return {height, width, heightmap};
}
