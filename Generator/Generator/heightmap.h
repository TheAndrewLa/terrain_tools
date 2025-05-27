#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include "../Types.h"
#include <cmath>
using namespace types;

namespace terraingenerator {
    template <std::floating_point U>
    struct HeightMap
    {
        uint32 height = 512;
        uint32 width = 512;
        uint32 x_offset = 0;
        uint32 y_offset = 0;
        U scale = 1;
        U angle = 0;
        U amplitude = 1;
        uint32 octaves = 1;
        uint32 seed = 0;
    };
}
#endif // HEIGHTMAP_H
