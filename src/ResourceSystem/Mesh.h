#pragma once

#ifndef __MESH_H__
#define __MESH_H__

#include "BaseHeader.h"

struct Vertex
{
    vec3 pos;
    vec3 norm;
    vec2 uv;
};

struct SubMesh
{
    String name;
    int vertexOffset;
    int indexOffset;
    int indexCount;
};

class Mesh
{
public:
    static SPtr<Mesh> Load(const char* path);

public:
    Mesh(int vertexNum, int indexNum);
    ~Mesh();

    void SetVertices(Vertex* pVertices, int numVertices);
    void SetIndices(uint* pIndices, int numIndices);
    void SetSubMesh(SubMesh* pSubMeshes, int numSubMeshes);
    void UpdateVertexArray();

    GLuint GetVertexArray() const
    {
        return mVertexArray;
    }
    int GetIndicesNum() const { return mIndexNum; }
    const Vector<SubMesh>& GetSubMeshes() const { return mSubMeshes; }
    GLuint GetIndexBuffer() const { return mIndexBuffer; }

private:
    int mVertexNum;
    int mIndexNum;
    int mTotalSize;
    void* mpData; // Combine vertex / index

    Vector<SubMesh> mSubMeshes;

    GLuint mVertexBuffer;
    GLuint mIndexBuffer;
    GLuint mVertexArray;
};

#endif // __MESH_H__
