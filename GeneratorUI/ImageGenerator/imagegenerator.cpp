#include "imagegenerator.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image_write.h"

void ImageGenerator::export_png(Terrain& terrain, const char *file_name)
{
    auto out = new char[terrain.height * terrain.width];
    for (usize i = 0; i < terrain.height; i++) {
        for (usize j = 0; j < terrain.width; j++) {
            out[i * terrain.height + j] = terrain.heightmap[i][j];
        }
    }

    stbi_write_png(file_name, (int) terrain.height, (int) terrain.width,
                   1, out, terrain.height);
    delete[] out;
}
