#pragma once

#ifndef __BASE_MESH_GENERATOR_H__
#define __BASE_MESH_GENERATOR_H__

#include "BaseHeader.h"
#include "Mesh.h"

class BaseMeshGenerator
{
public:
    BaseMeshGenerator() = delete;
    ~BaseMeshGenerator() = delete;

    static SPtr<Mesh> GetBoxMesh();
    static SPtr<Mesh> GetSphereMesh(int div = 72);
    static SPtr<Mesh> GetCapsuleMesh();
    static SPtr<Mesh> GetPlaneMesh();

private:
    static void SetNormalVector(Vector<Vertex>& vertices, Vector<uint>& indices);
};

#endif // __BASE_MESH_GENERATOR_H__
