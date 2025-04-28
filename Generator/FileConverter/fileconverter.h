#ifndef FILECONVERTER_H
#define FILECONVERTER_H

#pragma once
#include "../Generator/terrain.h"
#include <fstream>

namespace terraingenerator {
    class FileConverter
    {
    private:
        static constexpr uint32 COUNT_CHAR = 256;

        template <typename T>
        static void write_freq(uint32* freq, std::ofstream& output_file) {
            T* new_freq = new T[COUNT_CHAR];
            for (uint32 i = 0; i < COUNT_CHAR; ++i) {
                new_freq[i] = static_cast<T>(freq[i]);
            }
            output_file.write(reinterpret_cast<char*>(new_freq), COUNT_CHAR * sizeof(T));
            delete[] new_freq;
        }

        template <typename T>
        static uint32 read_freq(uint32* freq, std::ifstream& input_file) {
            T* new_freq = new T[COUNT_CHAR];
            input_file.read(reinterpret_cast<char*>(new_freq), COUNT_CHAR * sizeof(T));
            for (uint32 i = 0; i < COUNT_CHAR; ++i) {
                freq[i] = static_cast<uint32>(new_freq[i]);
            }
            delete[] new_freq;
            return sizeof(T);
        }

    public:
        FileConverter();
        static void to_file(Terrain& terrain, const char* file_name);
        static Terrain from_file(const char* file_name);
    };
}

#endif // FILECONVERTER_H
