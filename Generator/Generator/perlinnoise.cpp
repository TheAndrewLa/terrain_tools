#include "perlinnoise.h"

using namespace terraingenerator;

PerlinNoise::PerlinNoise() {
}

double PerlinNoise::linear_interpolate(double a, double b, double x) {
    return a + x * (b - a);
}

double PerlinNoise::cosine_interpolate(double a, double b, double x) {
    double f = (1 - cos(x * M_PI)) * .5;
    return  a*(1-f) + b*f;
}

double PerlinNoise::fade(double x) {
    return ((6 * x - 15) * x + 10) * x * x * x;
}

double PerlinNoise::gradient(int hash, double x, double y) {
    int h = hash & 15;
    double u = h < 8 ? x : y;
    double v = h < 4 ? y : (h == 12 || h == 14) ? x : 0;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

double PerlinNoise::smoothNoise(double x, double y) {
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

    double x1 = cosine_interpolate(gradient(aa, x_fractional_part, y_fractional_part),
                                   gradient(ba, x_fractional_part - 1, y_fractional_part), u);
    double y1 = cosine_interpolate(gradient(ab, x_fractional_part, y_fractional_part - 1),
                                   gradient(bb, x_fractional_part - 1, y_fractional_part - 1), u);
    return (cosine_interpolate(x1, y1, v) + 1) / 2;
}

double PerlinNoise::noise2D(double x, double y, int octaves, double amplitude) {
    double total = 0;
    double frequency = 0.7;
    double max_value = 0;

    for (int i = 0; i < octaves; ++i) {
        total += smoothNoise(x * frequency, y * frequency) * amplitude;

        max_value += amplitude;

        amplitude *= 0.5f;
        frequency *= 2.0f;
    }
    return total / max_value;
}
