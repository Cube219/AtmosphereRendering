#include "FbxImporter.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

Assimp::Importer FbxImporter::mImporter;

SPtr<Mesh> FbxImporter::Import(const char* path)
{
    const aiScene* scene = mImporter.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if(!scene || !(scene->mRootNode)) {
        return nullptr;
    }

    int verticesNum = 0, indicesNum = 0;
    for(uint i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* sMesh = scene->mMeshes[i];
        verticesNum += sMesh->mNumVertices;
        indicesNum += sMesh->mNumFaces * sMesh->mFaces[0].mNumIndices;
    }

    Vector<Vertex> vertices;
    Vector<uint> indices;
    Vector<SubMesh> subMeshes;
    vertices.reserve(verticesNum);
    indices.reserve(indicesNum);

    uint currentIndicesOffset = 0;

    for(uint i = 0; i < scene->mNumMeshes; i++) {
        currentIndicesOffset = vertices.size();

        aiMesh* sMesh = scene->mMeshes[i];

        SubMesh subMesh;
        subMesh.name = sMesh->mName.C_Str();
        subMesh.vertexOffset = vertices.size();
        subMesh.indexOffset = indices.size();
        subMesh.indexCount = sMesh->mNumFaces * sMesh->mFaces[0].mNumIndices;

        for(uint j = 0; j < sMesh->mNumVertices; j++) {
            Vertex v;
            
            if(sMesh->HasPositions() == true) {
                aiVector3D position = sMesh->mVertices[j];
                v.pos = vec3(position.x, position.y, position.z);
            }

            if(sMesh->HasNormals() == true) {
                aiVector3D normal = sMesh->mNormals[j];
                v.norm = vec3(normal.x, normal.y, normal.z);
            }

            if(sMesh->HasTextureCoords(0) == true) {
                aiVector3D uv = sMesh->mTextureCoords[0][j];
                v.uv = vec2(uv.x, uv.y);
            }

            vertices.push_back(v);
        }

        for(uint j = 0; j < sMesh->mNumFaces; j++) {
            aiFace face = sMesh->mFaces[j];

            for(uint k = 0; k < face.mNumIndices; k++) {
                indices.push_back(face.mIndices[k] + currentIndicesOffset);
            }
        }

        subMeshes.push_back(subMesh);
    }

    mImporter.FreeScene();

    SPtr<Mesh> mesh = std::make_shared<Mesh>(verticesNum, indicesNum);
    mesh->SetVertices(vertices.data(), vertices.size());
    mesh->SetIndices(indices.data(), indices.size());
    mesh->UpdateVertexArray();

    mesh->SetSubMesh(subMeshes.data(), subMeshes.size());

    return mesh;
}
