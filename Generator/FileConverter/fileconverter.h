#ifndef FILECONVERTER_H
#define FILECONVERTER_H

#pragma once
#include "../Generator/terrain.h"

namespace terraingenerator {
    class FileConverter
    {
    public:
        FileConverter();
        static void to_file(Terrain& terrain, const char* file_name);
        static Terrain from_file(const char* file_name);
    };
}

#endif // FILECONVERTER_H
