#ifndef IMAGEGENERATOR_H
#define IMAGEGENERATOR_H

#pragma once
#include <QImage>
#include "../Generator/terrain.h"

using namespace types;

namespace terraingenerator {
    template<std::unsigned_integral T, std::floating_point U>
    class ImageGenerator {
        static constexpr T max_T = std::numeric_limits<T>::max();

    public:
        enum class Biome : uint32 {
            OCEAN                     = 0x44447A,  // Темно-синий
            BEACH                     = 0xF0E68C,  // Песочный
            SCORCHED                  = 0x555555,  // Серый
            BARE                      = 0x888888,  // Светло-серый
            TUNDRA                    = 0xDDDDAF,  // Светло-бежевый
            SNOW                      = 0xFFFFFF,  // Белый
            TEMPERATE_DESERT          = 0xEDC9AF,  // Пустынный
            SHRUBLAND                 = 0xC4D4A3,  // Светло-зеленый
            TAIGA                     = 0x99AE7E,  // Зеленый тайги
            GRASSLAND                 = 0x88AA54,  // Травяной зеленый
            TEMPERATE_DECIDUOUS_FOREST= 0x679459,  // Лесной зеленый
            TEMPERATE_RAIN_FOREST     = 0x4F8053,  // Темно-зеленый
            SUBTROPICAL_DESERT        = 0xD2B98B,  // Песочный пустыни
            TROPICAL_SEASONAL_FOREST  = 0x5A8F4C,  // Тропический зеленый
            TROPICAL_RAIN_FOREST      = 0x3D6A3D
        };

        static Biome biome(U e, U m) {
            if (e < 0.15) return Biome::OCEAN;
            if (e < 0.2) return Biome::BEACH;

            if (e > 0.8) {
                if (m < 0.2) return Biome::SCORCHED;
                if (m < 0.4) return Biome::BARE;
                if (m < 0.7) return Biome::TUNDRA;
                return Biome::SNOW;
            }
            if (e > 0.6) {
                if (m < 0.33) return Biome::TEMPERATE_DESERT;
                if (m < 0.66) return Biome::SHRUBLAND;
                return Biome::TAIGA;
            }

            if (e > 0.4) {
                if (m < 0.16) return Biome::TEMPERATE_DESERT;
                if (m < 0.50) return Biome::GRASSLAND;
                if (m < 0.83) return Biome::TEMPERATE_DECIDUOUS_FOREST;
                return Biome::TEMPERATE_RAIN_FOREST;
            }

            if (m < 0.16) return Biome::SUBTROPICAL_DESERT;
            if (m < 0.33) return Biome::GRASSLAND;
            if (m < 0.66) return Biome::TROPICAL_SEASONAL_FOREST;
            return Biome::TROPICAL_RAIN_FOREST;
        }

        static void export_png(const Terrain<T>& terrain, const Terrain<T>& moisture_map, const char* file_name) {
            const size_t width = terrain.width();
            const size_t height = terrain.height();

            QImage image(width, height, QImage::Format_ARGB32);

            for (size_t y = 0; y < height; ++y) {
                for (size_t x = 0; x < width; ++x) {
                    Biome b = biome(1.0 * terrain[x][y] / max_T, 1.0 * moisture_map[x][y] / max_T);
                    image.setPixel(x, y, static_cast<uint32_t>(b) | 0xFF000000);
                }
            }
            image.save(QString::fromUtf8(file_name), "PNG");
        }
    };
}
#endif // IMAGEGENERATOR_H
