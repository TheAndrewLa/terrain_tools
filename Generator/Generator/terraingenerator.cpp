#include "terraingenerator.h"

using namespace terraingenerator;

long TerrainGenerator::generate_random_seed()
{
    return dist_(generator_);
}

HeightMap TerrainGenerator::generate_random_map()
{
    HeightMap map;
    map.seed = dist_(generator_);
    map.amplitude = ((map.seed & 0xffff) / 10) * 0.01;
    map.octaves = 1 + (map.seed & 0xffff0000 >> 16) % 10;
    return map;
}

TerrainGenerator::TerrainGenerator() : generator_(rd_()) {}

Terrain TerrainGenerator::generate(HeightMap& map)
{   double cos_angle = std::cos(map.angle);
    double sin_angle = std::sin(map.angle);

    Terrain terrain{map.height, map.width};

    for (int x = 0; x < map.height; ++x) {
        for (int y = 0; y < map.width; ++y) {
            double nx = (x + map.x_offset);
            double ny = (y + map.y_offset);
            nx = (nx * cos_angle - ny * sin_angle) / map.scale;
            ny = (nx * sin_angle + ny * cos_angle) / map.scale;
            terrain[x][y] = static_cast<char>(noise_.noise2D(nx, ny, map.octaves, map.amplitude) * 255);
        }
    }
    return terrain;
}
