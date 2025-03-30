#include "terrain.h"

Terrain::Terrain() {}

Terrain::Terrain(int width, int height) : width(width), height(height) {
    heightmap = new char*[height];
    for (usize i = 0; i < height; ++i) {
        heightmap[i] = new char[width]();
    }
}

Terrain::Terrain(int width, int height, char** heightmap) :
    width(width), height(height), heightmap(heightmap) {}

Terrain::Terrain(Terrain&& other) :
    width(other.width), height(other.height), heightmap(other.heightmap) {
    other.heightmap = nullptr;
}

Terrain& Terrain::operator=(Terrain&& other) {
    if (this == &other) {
        return *this;
    }

    deleteHeightMap();

    width = other.width;
    height = other.height;
    heightmap = other.heightmap;

    other.heightmap = nullptr;

    return *this;
}

Terrain::operator std::string()
{
    std::string map_str = "";
    for (usize x = 0; x < height; ++x) {
        for (usize y = 0; y < width; ++y) {
            map_str += std::to_string(heightmap[x][y]) + "\t";
        }
        map_str += "\n";
    }
    return map_str;
}

void Terrain::deleteHeightMap() {
    if (heightmap == nullptr) {
        return;
    }
    for (int i = 0; i < height; ++i) {
        if (heightmap[i] != nullptr) {
            delete[] heightmap[i];
        }
    }
    delete[] heightmap;
}
Terrain::~Terrain() {
    deleteHeightMap();
}
