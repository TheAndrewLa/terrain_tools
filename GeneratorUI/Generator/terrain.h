#ifndef TERRAIN_H
#define TERRAIN_H

#include <string>
#include "../Types.h"

using namespace ala::types;

struct Terrain
{
    int width;
    int height;
    char** heightmap;
    Terrain();
    Terrain(int width, int height);
    Terrain(int width, int height, char** heightmap);
    Terrain(const Terrain& other) = delete;
    Terrain& operator=(const Terrain& other) = delete;
    Terrain(Terrain&& other);
    Terrain& operator=(Terrain&& other);
    operator std::string();
    ~Terrain();
private:
    void deleteHeightMap();
};

#endif // TERRAIN_H
