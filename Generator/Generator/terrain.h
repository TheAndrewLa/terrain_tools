#ifndef TERRAIN_H
#define TERRAIN_H

#pragma once
#include <string>
#include "../Types.h"

using namespace types;

namespace terraingenerator {
    template <std::unsigned_integral T>
    class FileConverter;

    template <std::unsigned_integral T>
    class Terrain
    {
        struct Array {
            T *array = nullptr;
            usize start;

            Array(T *array, usize start) : array(array), start(start) {
            }

            T &operator[](usize y) {
                return array[start + y];
            }
        };

    protected:
        uint32 width_;
        uint32 height_;
        T* map_ = nullptr;

        void deleteMap() {
            if (map_ != nullptr) {
                delete[] map_;
            }
        }

    public:
        Terrain() = default;

        Terrain(uint32 width, uint32 height) : Terrain(width, height, new uchar[height * width]) {
        }

        Terrain(uint32 width, uint32 height, uchar* map) : width_(width), height_(height), map_(map) {
        }

        Terrain(const Terrain& other) = delete;

        Terrain& operator=(const Terrain& other) = delete;

        Terrain(Terrain&& other) :
            width_(other.width_), height_(other.height_), map_(other.map_) {
            other.map_ = nullptr;
        }

        uint32 width() const {
            return width_;
        }

        uint32 height() const {
            return height_;
        }

        T* map() const {
            return map_;
        }

        Terrain& operator=(Terrain&& other) {
            if (this == &other) {
                return *this;
            }

            deleteMap();

            width_ = other.width_;
            height_ = other.height_;
            map_ = other.map_;

            other.map_ = nullptr;

            return *this;
        }

        operator std::string()
        {
            std::string map_str = "";
            for (usize x = 0; x < height_; ++x) {
                for (usize y = 0; y < width_; ++y) {
                    map_str += std::to_string(map_[height_ * x + y]) + "\t";
                }
                map_str += "\n";
            }
            return map_str;
        }

        Array operator[](usize x) const
        {
            Array array{map_, height_ * x};
            return array;
        }

        ~Terrain() {
            deleteMap();
        }

        friend class FileConverter<T>;
    };
}


#endif // TERRAIN_H
