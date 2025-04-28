#ifndef TERRAIN_H
#define TERRAIN_H

#pragma once
#include <string>
#include "../Types.h"

using namespace types;

namespace terraingenerator {
    class Terrain
    {
        struct Array {
            uchar *array = nullptr;
            usize start;

            Array(uchar *array, usize start) : array(array), start(start) {
            }

            uchar &operator[](usize y) {
                return array[start + y];
            }
        };

        friend void export_png(const Terrain& terrain, const char *file_name);
    private:
        uint32 width_;
        uint32 height_;
        uchar* heightmap_ = nullptr;
        void deleteHeightMap();

    public:
        Terrain();
        Terrain(uint32 width, uint32 height);
        Terrain(uint32 width, uint32 height, uchar* map);
        Terrain(const Terrain& other) = delete;
        Terrain& operator=(const Terrain& other) = delete;
        Terrain(Terrain&& other);
        uint32 width() const;
        uint32 height() const;
        uchar* heightmap() const;
        Terrain& operator=(Terrain&& other);
        operator std::string();
        Array operator[](usize x) const;
        ~Terrain();
        friend class FileConverter;
    };
}


#endif // TERRAIN_H
