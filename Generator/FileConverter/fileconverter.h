#ifndef FILECONVERTER_H
#define FILECONVERTER_H

#pragma once
#include "../Generator/terrain.h"
#include "../FileConverter/huffmancompressor.h"
#include <fstream>
#include <ios>
#include <algorithm>
#include <memory>
#include <stdexcept>
#include <iostream>

namespace terraingenerator {
    template <std::unsigned_integral T>
    class FileConverter
    {
    private:
        template <std::unsigned_integral U>
        static void write_freq(uint32* freq, std::ofstream& output_file) {
            const uint32 count_U = std::numeric_limits<T>::max() + 1;
            U* new_freq = new U[count_U];
            for (uint32 i = 0; i < count_U; ++i) {
                new_freq[i] = static_cast<U>(freq[i]);
            }
            output_file.write(reinterpret_cast<char*>(new_freq), count_U * sizeof(U));
            delete[] new_freq;
        }

        template <std::unsigned_integral U>
        static uint32 read_freq(uint32* freq, std::ifstream& input_file) {
            const uint32 count_U = std::numeric_limits<T>::max() + 1;
            U* new_freq = new U[count_U];
            input_file.read(reinterpret_cast<char*>(new_freq), count_U * sizeof(U));
            for (uint32 i = 0; i < count_U; ++i) {
                freq[i] = static_cast<uint32>(new_freq[i]);
            }
            delete[] new_freq;
            return sizeof(U);
        }

    public:
        static void to_file(Terrain<T>& terrain, const char* file_name) {
            const uint32 count_U = std::numeric_limits<T>::max() + 1;
            size_t size = terrain.height_ * terrain.width_;
            std::unique_ptr<T[]> delta(new T[size]);

            delta[0] = terrain.map_[0];
            for (size_t i = 1; i < size; ++i) {
                delta[i] = terrain.map_[i] - terrain.map_[i - 1];
            }

            uint32 freq[count_U] = {0};
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

            HuffmanCommpressor<T> commpressor(freq);
            uint32 compressedSize;
            T* compressed = commpressor.compress(delta.get(), size, compressedSize, delta[0]);

            output_file.write(reinterpret_cast<char*>(&terrain.height_), sizeof(uint32));
            output_file.write(reinterpret_cast<char*>(&terrain.width_), sizeof(uint32));
            output_file.write(reinterpret_cast<char*>(&(delta[0])), sizeof(T));

            output_file.write(reinterpret_cast<char*>(&compressedSize), sizeof(uint32));
            output_file.write((char*)compressed, compressedSize);

            delete[] compressed;
        }

        static Terrain<T> from_file(const char* file_name) {
            const uint32 count_U = std::numeric_limits<T>::max() + 1;
            std::ifstream input_file(file_name, std::ios::binary);
            if (!input_file) {
                throw std::runtime_error("Failed to open input file");
            }
            uint32 max_freq;
            input_file.read(reinterpret_cast<char*>(&max_freq), sizeof(uint32));

            uint32 freq[count_U] = {};

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
            std::cout << height << " " << width << "\n";

            T first;
            input_file.read(reinterpret_cast<char*>(&first), sizeof(T));

            uint32 compressed_size;
            input_file.read(reinterpret_cast<char*>(&compressed_size), sizeof(uint32));
            std::unique_ptr<T[]> compressed(new T[compressed_size]);
            input_file.read(reinterpret_cast<char*>(compressed.get()), compressed_size);

            uint32 size = height * width;
            HuffmanCommpressor<T> commpressor(freq);
            T* heightmap = commpressor.decompress(compressed.get(), compressed_size, size, first);

            return {width, height, heightmap};
        }
    };
}

#endif // FILECONVERTER_H
