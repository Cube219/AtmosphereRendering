#pragma once
#ifndef __FBX_IMPORTER_H__
#define __FBX_IMPORTER_H__

#include "BaseHeader.h"

#include <assimp/Importer.hpp>

class Mesh;

class FbxImporter
{
public:
    FbxImporter() {}

    static SPtr<Mesh> Import(const char* path);

private:
    static Assimp::Importer mImporter;
};

#endif // __FBX_IMPORTER_H__
