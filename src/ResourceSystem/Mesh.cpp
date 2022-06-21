#include "Mesh.h"

#include <memory.h>

#include "FbxImporter.h"

SPtr<Mesh> Mesh::Load(const char* path)
{
    return FbxImporter::Import(path);
}

Mesh::Mesh(int vertexNum, int indexNum) :
    mVertexNum(vertexNum),
    mIndexNum(indexNum),
    mTotalSize(vertexNum * sizeof(Vertex) + indexNum * sizeof(int)),
    mVertexArray(0)
{
    mpData = malloc(mTotalSize);
    glGenBuffers(1, &mVertexBuffer);
    glGenBuffers(1, &mIndexBuffer);
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &mVertexBuffer);
    glDeleteBuffers(1, &mIndexBuffer);
    if(mVertexArray) {
        glDeleteVertexArrays(1, &mVertexArray);
        mVertexArray = 0;
    }
    free(mpData);
}

void Mesh::SetVertices(Vertex* pVertices, int numVertices)
{
    memcpy(mpData, pVertices, numVertices * sizeof(Vertex));
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * numVertices, pVertices, GL_STATIC_DRAW);
}

void Mesh::SetIndices(uint* pIndices, int numIndices)
{
    char* dst = (char*)mpData + (mVertexNum * sizeof(Vertex));
    memcpy(dst, pIndices, numIndices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * numIndices, pIndices, GL_STATIC_DRAW);
}

void Mesh::SetSubMesh(SubMesh* pSubMeshes, int numSubMeshes)
{
    mSubMeshes.clear();
    for(int i = 0; i < numSubMeshes; i++) {
        mSubMeshes.push_back(pSubMeshes[i]);
    }
}

void Mesh::UpdateVertexArray()
{
    if(mVertexArray) {
        glDeleteVertexArrays(1, &mVertexArray);
        mVertexArray = 0;
    }

    glGenVertexArrays(1, &mVertexArray);
    glBindVertexArray(mVertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

    size_t offset = 0;
    // 0: position
    glEnableVertexAttribArray((GLuint)0);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offset));
    offset += sizeof(Vertex::pos);
    // 1: norm
    glEnableVertexAttribArray((GLuint)1);
    glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offset));
    offset += sizeof(Vertex::norm);
    // 2: uv
    glEnableVertexAttribArray((GLuint)2);
    glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offset));
    offset += sizeof(Vertex::uv);

    glBindVertexArray(0);
}
