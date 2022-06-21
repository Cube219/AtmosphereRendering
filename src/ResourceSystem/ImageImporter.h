#pragma once

#ifndef __IMAGE_IMPORTER_H__
#define __IMAGE_IMPORTER_H__

#include "BaseHeader.h"

class Texture;

class ImageImporter
{
public:
    static SPtr<Texture> Import(const char* path);
};

#endif // __IMAGE_IMPORTER_H__
