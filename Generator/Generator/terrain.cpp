#include "terrain.h"

using namespace terraingenerator;

Terrain::Terrain() {}

Terrain::Terrain(int width, int height) : width_(width), height_(height) {
    heightmap_ = new uchar[height * width];
}

Terrain::Terrain(Terrain&& other) :
    width_(other.width_), height_(other.height_), heightmap_(other.heightmap_) {
    other.heightmap_ = nullptr;
}

Terrain& Terrain::operator=(Terrain&& other) {
    if (this == &other) {
        return *this;
    }

    deleteHeightMap();

    width_ = other.width_;
    height_ = other.height_;
    heightmap_ = other.heightmap_;

    other.heightmap_ = nullptr;

    return *this;
}

Terrain::operator std::string()
{
    std::string map_str = "";
    for (usize x = 0; x < height_; ++x) {
        for (usize y = 0; y < width_; ++y) {
            map_str += std::to_string(heightmap_[height_ * x + y]) + "\t";
        }
        map_str += "\n";
    }
    return map_str;
}

Terrain::Array Terrain::operator[](usize x) const
{
    Array array{heightmap_, height_ * x};
    return array;
}

void Terrain::deleteHeightMap() {
    if (heightmap_ != nullptr) {
        delete[] heightmap_;
    }
}
Terrain::~Terrain() {
    deleteHeightMap();
}



