#include "fileconverter.h"

#include "../FileConverter/huffmancompressor.h"

#include <ios>
#include <algorithm>
#include <memory>
#include <stdexcept>
#include <iostream>

using namespace terraingenerator;

void FileConverter::to_file(Terrain &terrain, const char *file_name)
{
    size_t size = terrain.height_ * terrain.width_;
    std::unique_ptr<uchar[]> delta(new uchar[size]);

    delta[0] = terrain.heightmap_[0];
    for (size_t i = 1; i < size; ++i) {
        delta[i] = terrain.heightmap_[i] - terrain.heightmap_[i - 1];
    }

    uint32 freq[COUNT_CHAR] = {0};
    uint32 max_freq = 0;
    for (uint32 i = 1; i < size; ++i) {
        freq[delta[i]]++;
        max_freq = std::max(max_freq, freq[delta[i]]);
    }

    std::ofstream output_file(file_name, std::ios::binary);
    if (!output_file) {
        throw std::runtime_error("Failed to open output file");
    }

    output_file.write(reinterpret_cast<char*>(&max_freq), sizeof(uint32));

    if (max_freq < UINT8_MAX) {
        write_freq<uint8>(freq, output_file);
    } else if (max_freq < UINT16_MAX) {
        write_freq<uint16>(freq, output_file);
    }  else {
        write_freq<uint32>(freq, output_file);
    }

    HuffmanCommpressor commpressor(freq);
    uint32 compressedSize;
    uchar* compressed = commpressor.compress(delta.get(), size, compressedSize, delta[0]);

    output_file.write(reinterpret_cast<char*>(&terrain.height_), sizeof(uint32));
    output_file.write(reinterpret_cast<char*>(&terrain.width_), sizeof(uint32));
    output_file.put((char)delta[0]);

    output_file.write(reinterpret_cast<char*>(&compressedSize), sizeof(uint32));
    output_file.write((char*)compressed, compressedSize);

    delete[] compressed;
}


Terrain FileConverter::from_file(const char *file_name)
{
    std::ifstream input_file(file_name, std::ios::binary);
    if (!input_file) {
        throw std::runtime_error("Failed to open input file");
    }
    uint32 max_freq;
    input_file.read(reinterpret_cast<char*>(&max_freq), sizeof(uint32));

    uint32 freq[COUNT_CHAR] = {};

    if (max_freq < UINT8_MAX) {
        read_freq<uint8>(freq, input_file);
    } else if (max_freq < UINT16_MAX) {
        read_freq<uint16>(freq, input_file);
    }  else {
        read_freq<uint32>(freq, input_file);
    }

    uint32 height, width;
    input_file.read(reinterpret_cast<char*>(&height), sizeof(uint32));
    input_file.read(reinterpret_cast<char*>(&width), sizeof(uint32));

    uchar first;
    input_file.read(reinterpret_cast<char*>(&first), sizeof(uchar));

    uint32 compressed_size;
    input_file.read(reinterpret_cast<char*>(&compressed_size), sizeof(uint32));
    std::unique_ptr<uchar[]> compressed(new uchar[compressed_size]);
    input_file.read(reinterpret_cast<char*>(compressed.get()), compressed_size);

    uint32 size = height * width;
    HuffmanCommpressor commpressor(freq);
    uchar* heightmap = commpressor.decompress(compressed.get(), compressed_size, size, first);

    return {height, width, heightmap};
}
