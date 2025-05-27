#ifndef PERLINNOISE_H
#define PERLINNOISE_H

#include <cmath>

namespace terraingenerator {
    template<std::floating_point T>
    class PerlinNoise
    {
    private:
        static constexpr int permutation[] = { 151,160,137,91,90,15,
            131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
            190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
            88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
            77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
            102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
            135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
            5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
            223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
            129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
            251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
            49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
            138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
        };

        static T linear_interpolate(T a, T b, T x) {
            return a + x * (b - a);
        }

        static T cosine_interpolate(T a, T b, T x) {
            double f = (1 - cos(x * M_PI)) * .5;
            return  a*(1-f) + b*f;
        }

        static T fade(T x) {
            return ((6 * x - 15) * x + 10) * x * x * x;
        }

        static T gradient(int hash, T x, T y) {
            int h = hash & 15;
            double u = h < 8 ? x : y;
            double v = h < 4 ? y : (h == 12 || h == 14) ? x : 0;
            return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
        }

        static T smoothNoise(T x, T y) {
            int x_integer_part = static_cast<int>(x) & 255;
            int y_integer_part = static_cast<int>(y) & 255;
            double x_fractional_part = x - x_integer_part;
            double y_fractional_part = y - y_integer_part;

            double u = fade(x_fractional_part);
            double v = fade(y_fractional_part);
            int aa = permutation[(permutation[x_integer_part] + y_integer_part) & 255];
            int ba = permutation[(permutation[(x_integer_part + 1) & 255] + y_integer_part) & 255];
            int ab = permutation[(permutation[x_integer_part] + y_integer_part + 1) & 255];
            int bb = permutation[(permutation[(x_integer_part + 1) & 255] + y_integer_part + 1) & 255];

            double x1 = linear_interpolate(gradient(aa, x_fractional_part, y_fractional_part),
                                           gradient(ba, x_fractional_part - 1, y_fractional_part), u);
            double y1 = linear_interpolate(gradient(ab, x_fractional_part, y_fractional_part - 1),
                                           gradient(bb, x_fractional_part - 1, y_fractional_part - 1), u);
            return (linear_interpolate(x1, y1, v) + 1) / 2;
        }
    public:
        static T noise2D(T x, T y, int octaves, T amplitude) {
            double total = 0;
            double frequency = 0.01;
            double max_value = 0;

            for (int i = 0; i < octaves; ++i) {
                total += smoothNoise(x * frequency, y * frequency) * amplitude;

                max_value += amplitude;

                amplitude *= 0.5f;
                frequency *= 2.0f;
            }
            return total / max_value;
        }
    };
}

#endif // PERLINNOISE_H
