#include "terrain.h"

using namespace terraingenerator;

Terrain::Terrain() {}

Terrain::Terrain(int width, int height) : Terrain(width, height, new uchar[height * width]) {
}

Terrain::Terrain(int width, int height, uchar* map) : width_(width), height_(height), heightmap_(map) {
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

int Terrain::width() const {
    return width_;
}
int Terrain::height() const {
    return height_;
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



