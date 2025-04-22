#include "terraingenerator.h"

using namespace terraingenerator;

int TerrainGenerator::generate_random_seed()
{
    return dist_(generator_);
}

void TerrainGenerator::generate_random_map(HeightMap& map)
{
    map.seed = dist_(generator_);
    map.amplitude = ((map.seed & 0xffff) % 500) * 0.01;
    map.octaves = 1 + (map.seed & 0xffff0000 >> 16) % 5;
}

TerrainGenerator::TerrainGenerator() : generator_(rd_()) {}

Terrain TerrainGenerator::generate(HeightMap& map, Curve<double>& curve) {
    double radian_angle = M_PI * map.angle / 180;
    double cos_angle = std::cos(radian_angle);
    double sin_angle = std::sin(radian_angle);

    Terrain terrain{map.height, map.width};

    double offset = sqrt(map.height * map.height + map.width * map.width);
    double x_center = offset + map.height / 2;
    double y_center = offset + map.width / 2;

    for (size_t x = 0; x < map.height; ++x) {
        for (size_t y = 0; y < map.width; ++y) {
            double xx = x + offset;
            double yy = y + offset;

            double nx = map.x_offset + x_center + (xx - x_center) * cos_angle - (yy - y_center) * sin_angle;
            double ny = map.y_offset + y_center + (xx - x_center) * sin_angle + (yy - y_center) * cos_angle;

            nx = nx / map.scale;
            ny = ny / map.scale;

            double e = noise_.noise2D(nx, ny, map.octaves, map.amplitude);
            e = curve.calculate(e);
            terrain[x][y] = static_cast<uchar>(e * 255);

        }
    }
    return terrain;
}
