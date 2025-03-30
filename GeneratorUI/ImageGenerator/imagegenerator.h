#ifndef IMAGEGENERATOR_H
#define IMAGEGENERATOR_H

#pragma once
#include "../Generator/terrain.h"

class ImageGenerator
{
public:
    static void export_png(Terrain& terrain, const char *file_name);
};

#endif // IMAGEGENERATOR_H
