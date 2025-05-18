#ifndef IMAGEGENERATOR_H
#define IMAGEGENERATOR_H

#pragma once
#include "../Generator/terrain.h"

using namespace types;

namespace terraingenerator {
    class ImageGenerator {
    public:
        template <std::unsigned_integral T>
        static void export_png(const Terrain<T>& terrain, const char *file_name);
    //void saveTerrainAsPng(const Terrain& terrain, const char * filename);
    };
}
#endif // IMAGEGENERATOR_H
