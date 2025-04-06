#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H

#include "perlinnoise.h"
#include "heightmap.h"
#include "terrain.h"
#include <random>
#include <cmath>

namespace terraingenerator {
    class TerrainGenerator
    {
    private:
        PerlinNoise noise_;
        std::random_device rd_;
        std::mt19937 generator_;
        std::uniform_int_distribution<int> dist_;
    public:
        long generate_random_seed();
        TerrainGenerator();
        Terrain generate(HeightMap& map);
        HeightMap generate_random_map();
    };
}
#endif // TERRAINGENERATOR_H
