#include "terraingenerator.h"

long TerrainGenerator::generate_random_seed()
{
    return dist_(generator_);
}

HeightMap TerrainGenerator::generate_random_map()
{
    HeightMap map;
    map.seed = dist_(generator_);
    map.height = map.seed & 0xfff + 1;
    map.width = ((map.seed & 0xfff000) >>  12) + 1;
    map.amplitude = ((map.seed & 0xffff0 >> 4) % 20 + 1) * 0.5;
    map.octaves = 1 + map.seed % 10;
    map.scale = map.seed >> 2 & 8 + 1;
    map.x_offset = map.seed & 0xffff0000 >> 16;
    map.y_offset = map.seed & 0xffff0 >> 4;
    return map;
}

long TerrainGenerator::get_seed() const
{
    return seed_;
}

TerrainGenerator::TerrainGenerator() : generator_(rd_()) {}

Terrain TerrainGenerator::generate(HeightMap& map)
{
    char** heightmap = new char*[map.height];
    for (int x = 0; x < map.height; ++x) {
        heightmap[x] = new char[map.width];
        for (int y = 0; y < map.width; ++y) {
            double nx = (x + map.x_offset) / map.scale;
            double ny = (y + map.y_offset) / map.scale;
            heightmap[x][y] = static_cast<char>(noise_.noise2D(nx, ny, map.octaves, map.amplitude) * 255);
            //std::cout << PerlinNoise::perlin_noise(x + 0.7, y * 0.7) * map.amplitude  << " ";
            //heightmap[x][y] = (char)(PerlinNoise::perlin_noise(x + 0.7, y * 0.7) * map.amplitude * 256);
        }
    }
    return {map.width, map.height, heightmap};
}
