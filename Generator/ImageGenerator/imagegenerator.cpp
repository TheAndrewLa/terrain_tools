#include "imagegenerator.h"

using namespace terraingenerator;

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image_write.h"

void terraingenerator::export_png(const Terrain& terrain, const char *file_name)
{
    stbi_write_png(file_name, terrain.height_, terrain.width_,
                   1, terrain.heightmap_, terrain.height_);
}

