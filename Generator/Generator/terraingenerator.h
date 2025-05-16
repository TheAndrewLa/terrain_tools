#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H

#include "perlinnoise.h"
#include "heightmap.h"
#include "terrain.h"
#include "Curve.h"
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

        struct ErosionParams {
            size_t iterations = 100;
            double capacity = 4.0f;
            double deposition = 0.1f;
            double erosion = 0.3f;
            double evaporation = 0.01f;
        };

        void apply_erosion(Terrain& terrain, const ErosionParams& params);
        void carve_rivers(Terrain& terrain, float river_strength = 5.0f, float river_threshold = 0.4f);
        void apply_karst_erosion(Terrain& terrain, int num_holes = 50);

    public:
        bool appled_erosion = false;
        int generate_random_seed();
        TerrainGenerator();
        Terrain generate(HeightMap& map, Curve<double>& curve);
        void generate_random_map(HeightMap& map);
    };
}
#endif // TERRAINGENERATOR_H
