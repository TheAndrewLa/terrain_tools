#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

namespace terraingenerator {
    struct HeightMap
    {
        int height = 512;
        int width = 512;
        int x_offset = 0;
        int y_offset = 0;
        int scale = 1;
        double angle = 0;
        double amplitude = 1;
        int octaves = 1;
        int seed = 0;
    };
}
#endif // HEIGHTMAP_H
