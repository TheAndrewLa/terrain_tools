#include "terraingenerator.h"

#include <iostream>

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

    double x_offset = map.height * sqrt(2) + map.x_offset;
    double y_offset = map.width * sqrt(2) + map.x_offset;

    for (int x = 0; x < map.height; ++x) {
        for (int y = 0; y < map.width; ++y) {
            double xx = x + x_offset;
            double yy = y + y_offset;

            double nx = x_offset + (xx - x_offset) * cos_angle - (yy - y_offset) * sin_angle;
            double ny = y_offset + (xx - x_offset) * sin_angle + (yy - y_offset) * cos_angle;

            nx = nx / map.scale;
            ny = ny / map.scale;

            terrain[x][y] = static_cast<char>(noise_.noise2D(nx, ny, map.octaves, map.amplitude) * 255);
        }
    }
    return terrain;
}
