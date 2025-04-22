#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include "../Types.h"
using namespace types;

namespace terraingenerator {
    struct HeightMap
    {
        uint32 height = 512;
        uint32 width = 512;
        uint32 x_offset = 0;
        uint32 y_offset = 0;
        double scale = 1;
        double angle = 0;
        double amplitude = 1;
        uint32 octaves = 1;
        uint32 seed = 0;
    };
}
#endif // HEIGHTMAP_H
