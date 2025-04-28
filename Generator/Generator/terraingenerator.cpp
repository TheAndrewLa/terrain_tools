#include "terraingenerator.h"
#include <algorithm>

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
    ErosionParams params{};

    if (appled_erosion) {
        apply_erosion(terrain, params);
        carve_rivers(terrain, 0.01, 0.2);
    }
    return terrain;
}


void TerrainGenerator::apply_erosion(Terrain& terrain, const ErosionParams& params)
{
    const size_t width = terrain.width();
    const size_t height = terrain.height();

    std::vector<double> heightmap(height * width);
    std::vector<double> water(height * width, 0);
    std::vector<double> sediment(height * width, 0);
    std::vector<double> velocity(height * width, 0);

    for (size_t x = 0; x < height; ++x) {
        for (size_t y = 0; y < width; ++y) {
        heightmap[x * height + y] = terrain[x][y] / 255.0;
        }
    }

    auto index = [&](size_t x, size_t y) { return x * height + y; };

    const double rain_amount = 0.01;

    for (size_t iter = 0; iter < params.iterations; ++iter) {
        for (size_t i = 0; i < width * height; ++i) {
            water[i] += rain_amount;
        }
        for (size_t x = 1; x < height - 1; ++x) {
            for (size_t y = 1; y < width - 1; ++y) {
                size_t i = index(x, y);
                double current_height = heightmap[i] + water[i];

                double total_flux = 0.0;
                double flux[] = {0, 0, 0, 0};
                int dx[] = {1, -1, 0, 0};
                int dy[] = {0, 0, 1, -1};

                for (size_t d = 0; d < 4; ++d) {
                    size_t nx = x + dx[d];
                    size_t ny = y + dy[d];
                    size_t ni = index(nx, ny);

                    double neighbor_height = heightmap[ni] + water[ni];
                    double delta = current_height - neighbor_height;

                    if (delta > 0) {
                        flux[d] = delta;
                        total_flux += delta;
                    } else {
                        flux[d] = 0;
                    }
                }

                if (total_flux == 0){
                    continue;
                }

                for (size_t d = 0; d < 4; ++d) {
                    if (flux[d] > 0) {
                        int nx = x + dx[d];
                        int ny = y + dy[d];
                        size_t ni = index(nx, ny);

                        double amount = (flux[d] / total_flux) * water[i] * 0.5;

                        water[i] -= amount;
                        water[ni] += amount;

                        double slope = (heightmap[i] - heightmap[ni]);
                        double erosion_amount = std::min(slope * params.erosion, heightmap[i]);
                        heightmap[i] -= erosion_amount;
                        sediment[i] += erosion_amount;

                        double capacity = slope * water[i] * params.capacity;
                        if (sediment[i] > capacity) {
                            double deposit = (sediment[i] - capacity) * params.deposition;
                            heightmap[i] += deposit;
                            sediment[i] -= deposit;
                        }
                    }
                }
                water[i] *= (1.0f - params.evaporation);
            }
        }
    }
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
        terrain[i][j] = static_cast<uchar>(std::clamp(heightmap[i * height + j], 0.0, 1.0) * 255);
        }
    }
}


void TerrainGenerator::carve_rivers(Terrain& terrain, float river_strength, float river_threshold) {
    uint32 width = terrain.width();
    uint32 height = terrain.height();

    for (uint32 x = 1; x < height - 1; ++x) {
        for (uint32 y = 1; y < width - 1; ++y) {
            uchar height_value = terrain[x][y];

            float normalized = height_value / 255.0f;

            if (normalized < river_threshold) {
                float new_height = normalized - river_strength;
                new_height = std::max(0.0f, new_height);

                terrain[x][y] = static_cast<uchar>(new_height * 255);
            }
        }
    }
}

void TerrainGenerator::apply_karst_erosion(Terrain& terrain, int num_holes) {
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> dist_x(0, terrain.height() - 1);
    std::uniform_int_distribution<int> dist_y(0, terrain.width() - 1);
    std::uniform_int_distribution<int> dist_radius(5, 15);

    for (int i = 0; i < num_holes; ++i) {
        int cx = dist_x(gen);
        int cy = dist_y(gen);
        int radius = dist_radius(gen);

        for (int x = std::max(0, cx - radius); x < std::min<int>(terrain.height(), cx + radius); ++x) {
            for (int y = std::max(0, cy - radius); y < std::min<int>(terrain.width(), cy + radius); ++y) {
                float dx = x - cx;
                float dy = y - cy;
                if (dx * dx + dy * dy <= radius * radius) {
                    uchar& h = terrain[x][y];
                    h = static_cast<uchar>(std::max(0, h - 30));
                }
            }
        }
    }
}
