#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H

#include "perlinnoise.h"
#include "heightmap.h"
#include "terrain.h"
#include "Curve.h"
#include <random>
#include <cmath>
#include <limits>
#include <algorithm>

namespace terraingenerator {
    template<std::unsigned_integral T, std::floating_point U>
    class TerrainGenerator
    {
    private:
        std::random_device rd_;
        std::mt19937 generator_;
        std::uniform_int_distribution<int> dist_;
        static constexpr T max_T = std::numeric_limits<T>::max();
        static constexpr U max_U = static_cast<U>(max_T);

        struct ErosionParams {
            size_t iterations = 100;
            U capacity = 3.0f;
            U deposition = 0.1f;
            U erosion = 0.3f;
            U evaporation = 0.02f;
        };

        static U linear_interpolate(U a, U b, U x) {
            return a + x * (b - a);
        }

        void apply_erosion(Terrain<T>& terrain, const ErosionParams& params)
        {
            const size_t width = terrain.width();
            const size_t height = terrain.height();

            std::vector<U> heightmap(height * width);
            std::vector<U> water(height * width, 0);
            std::vector<U> sediment(height * width, 0);


            auto index = [&](size_t x, size_t y) { return x * height + y; };

            for (size_t x = 0; x < height; ++x) {
                for (size_t y = 0; y < width; ++y) {
                    heightmap[index(x, y)] = terrain[x][y] / 255.0;
                }
            }

            const U rain_amount = 0.01;

            for (size_t iteretion = 0; iteretion < params.iterations; ++iteretion) {
                for (size_t i = 0; i < width * height; ++i) {
                    water[i] += rain_amount;
                }
                for (size_t x = 1; x < height - 1; ++x) {
                    for (size_t y = 1; y < width - 1; ++y) {
                        size_t i = index(x, y);
                        U current_height = heightmap[i] + water[i];

                        U total_flux = 0.0;
                        U flux[] = {0, 0, 0, 0};
                        int dx[] = {1, -1, 0, 0};
                        int dy[] = {0, 0, 1, -1};

                        for (size_t d = 0; d < 4; ++d) {
                            size_t nx = x + dx[d];
                            size_t ny = y + dy[d];
                            size_t ni = index(nx, ny);

                            U neighbor_height = heightmap[ni] + water[ni];
                            U delta = current_height - neighbor_height;

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

                                U amount = (flux[d] / total_flux) * water[i] * 0.5;

                                water[i] -= amount;
                                water[ni] += amount;

                                U slope = (heightmap[i] - heightmap[ni]);
                                U erosion_amount = std::min(slope * params.erosion, heightmap[i]);
                                heightmap[i] -= erosion_amount;
                                sediment[ni] += erosion_amount;

                                U capacity = slope * water[ni] * params.capacity;
                                // if (sediment[ni] > capacity) {
                                //     U deposit = (sediment[ni] - capacity) * params.deposition;
                                //     heightmap[ni] += deposit;
                                //     sediment[ni] -= deposit;
                                // }
                                if (sediment[ni] > capacity) {
                                    sediment[ni] -= (sediment[ni] - capacity) * params.deposition;
                                }
                            }
                        }
                        water[i] *= (1.0f - params.evaporation);
                    }
                }
            }
            for (size_t i = 0; i < height; ++i) {
                for (size_t j = 0; j < width; ++j) {
                    terrain[i][j] = static_cast<T>(std::clamp<U>(heightmap[i * height + j], 0.0, 1.0) * 255);
                }
            }
        }

        void carve_rivers(Terrain<T>& terrain, U river_strength = 5.0, U river_threshold = 0.4f) {
            const size_t width = terrain.width();
            const size_t height = terrain.height();

            for (size_t x = 1; x < height - 1; ++x) {
                for (size_t y = 1; y < width - 1; ++y) {
                    T height_value = terrain[x][y];

                    U normalized = height_value / max_U;

                    if (normalized < river_threshold) {
                        U new_height = normalized - river_strength;
                        new_height = std::max(static_cast<U>(0.0), new_height);

                        terrain[x][y] = static_cast<T>(new_height * max_T);
                    }
                }
            }
        }

        void apply_karst_erosion(Terrain<T>& terrain, size_t num_holes = 50) {
            std::mt19937 gen(std::random_device{}());
            std::uniform_int_distribution<int> dist_x(0, terrain.height() - 1);
            std::uniform_int_distribution<int> dist_y(0, terrain.width() - 1);
            std::uniform_int_distribution<int> dist_radius(5, 15);

            for (size_t i = 0; i < num_holes; ++i) {
                int cx = dist_x(gen);
                int cy = dist_y(gen);
                int radius = dist_radius(gen);

                for (int x = std::max(0, cx - radius); x < std::min<int>(terrain.height(), cx + radius); ++x) {
                    for (int y = std::max(0, cy - radius); y < std::min<int>(terrain.width(), cy + radius); ++y) {
                        U dx = x - cx;
                        U dy = y - cy;
                        if (dx * dx + dy * dy <= radius * radius) {
                            T& h = terrain[x][y];
                            h = static_cast<T>(std::max(0, h - 30));
                        }
                    }
                }
            }
        }


    public:
        bool appled_erosion = false;

        int generate_random_seed() {
            return dist_(generator_);
        }

        TerrainGenerator() : generator_(rd_()) {}

        Terrain<T> generate(HeightMap<U>& map, Curve<U>& curve) {
            U radian_angle = M_PI * map.angle / 180;
            U cos_angle = std::cos(radian_angle);
            U sin_angle = std::sin(radian_angle);

            Terrain<T> terrain{map.height, map.width};

            U offset = sqrt(map.height * map.height + map.width * map.width);
            U x_center = offset + map.height / 2;
            U y_center = offset + map.width / 2;

            for (size_t x = 0; x < map.height; ++x) {
                for (size_t y = 0; y < map.width; ++y) {
                    U xx = x + offset;
                    U yy = y + offset;

                    U nx = map.x_offset + x_center + (xx - x_center) * cos_angle - (yy - y_center) * sin_angle;
                    U ny = map.y_offset + y_center + (xx - x_center) * sin_angle + (yy - y_center) * cos_angle;


                    U nx1 = 2*x/map.height - 1;
                    U ny1 = 2*y/map.width - 1;
                    //U d = std::min(1.0, (nx1 * nx1 + ny1 * ny1) / sqrt(2));
                    U d = 1 - (1 - nx1 * nx1) * ( 1 - ny1 * ny1);
                    nx = nx / map.scale;
                    ny = ny / map.scale;

                    U e = PerlinNoise<U>::noise2D(nx, ny, map.octaves, map.amplitude);
                    e = linear_interpolate(e, 1 - d, 0);
                    e = curve.calculate(e);
                    terrain[x][y] = static_cast<T>(e * max_T);
                }
            }
            ErosionParams params{};

            if (appled_erosion) {
                apply_erosion(terrain, params);
                carve_rivers(terrain, 0.01, 0.2);
            }
            return terrain;
        }

        void generate_random_map(HeightMap<U>& map) {
            map.seed = dist_(generator_);
            map.amplitude = ((map.seed & 0xffff) % 5000) * 0.01;
            map.octaves = 1 + (map.seed & 0xffff0000 >> 16) % 10;
        }
    };
}
#endif // TERRAINGENERATOR_H
