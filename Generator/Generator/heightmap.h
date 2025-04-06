#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

namespace terraingenerator {
    struct HeightMap
    {
        int height;
        int width;
        int x_offset;
        int y_offset;
        int scale;
        double angle;
        double amplitude;
        int octaves;
        int seed;
    };
}
#endif // HEIGHTMAP_H
