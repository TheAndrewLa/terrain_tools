#include "imagegenerator.h"

using namespace terraingenerator;

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image_write.h"


template <>
void ImageGenerator::export_png<uchar>(const Terrain<uchar>& terrain, const char *file_name) {
    stbi_write_png(file_name, terrain.height(), terrain.width(), 1, terrain.heightmap(), terrain.height());
}
