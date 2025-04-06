#ifndef IMAGEGENERATOR_H
#define IMAGEGENERATOR_H

#pragma once
#include "../Types.h"
#include "../Generator/terrain.h"

using namespace ala::types;

namespace terraingenerator {
    void export_png(const Terrain& terrain, const char *file_name);
}
#endif // IMAGEGENERATOR_H
