#ifndef TERRAIN_H
#define TERRAIN_H

#pragma once
#include <string>
#include "../Types.h"

using namespace ala::types;

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
        int width_;
        int height_;
        uchar* heightmap_;
        void deleteHeightMap();

    public:
        Terrain();
        Terrain(int width, int height);
        Terrain(const Terrain& other) = delete;
        Terrain& operator=(const Terrain& other) = delete;
        Terrain(Terrain&& other);
        int weigth() const;
        int height() const;
        Terrain& operator=(Terrain&& other);
        operator std::string();
        Array operator[](usize x) const;
        ~Terrain();
    };
}


#endif // TERRAIN_H
