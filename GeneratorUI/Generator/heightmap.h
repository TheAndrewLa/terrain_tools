#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

struct HeightMap
{
    int height;
    int width;
    int x_offset;
    int y_offset;
    int scale;
    int amplitude;
    int octaves;
    int seed;
};

#endif // HEIGHTMAP_H
