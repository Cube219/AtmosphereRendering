#pragma once

#ifndef __RENDERER_COMPONENT_H__
#define __RENDERER_COMPONENT_H__

#include "BaseHeader.h"

#include "../GameObjectSystem/GameObject.h"
#include "../ResourceSystem/Mesh.h"

class Texture;

struct Material
{
    vec4 ambient = vec4(0);
    vec4 diffuse = vec4(0);
    vec4 specular = vec4(0);

    SPtr<Texture> tAmbient = nullptr;
    SPtr<Texture> tDiffuse = nullptr;
    SPtr<Texture> tSpecular = nullptr;
    SPtr<Texture> tNormal = nullptr;

    void Release()
    {
        tAmbient = nullptr;
        tDiffuse = nullptr;
        tSpecular = nullptr;
        tNormal = nullptr;
    }
};

class RendererComponent
{
public:
    RendererComponent(GameObject& ownerGameObject) :
        mMesh(nullptr),
        mOwnerGameObject(ownerGameObject),
        mUseShading(true),
        mIsVisible(true)
    {
    }
    virtual ~RendererComponent() {}

    void SetMesh(SPtr<Mesh> mesh)
    {
        mMesh = mesh;
        mMaterials.resize(mMesh->GetSubMeshes().size());
    }

    void SetMaterial(int index, Material* material)
    {
        mMaterials[index] = material;
    }

    void SetUseShading(bool useShading) { mUseShading = useShading; }
    void SetVisible(bool isVisible) { mIsVisible = isVisible; }

    SPtr<Mesh> GetMesh() const { return mMesh; }

    const Vector<Material*>& GetMaterials() const { return mMaterials; }

    const mat4& GetModelMatrix() const
    {
        return mOwnerGameObject.GetModelMatrix();
    }

    vec4 GetDebugColor() const
    {
        return mOwnerGameObject.GetDebugColor();
    }

    bool GetUseShading() const { return mUseShading; }
    bool GetIsVisible() const { return mIsVisible; }

private:
    SPtr<Mesh> mMesh;
    Vector<Material*> mMaterials;
    GameObject& mOwnerGameObject;
    bool mUseShading;
    bool mIsVisible;
};

#endif // __RENDERER_COMPONENT_H__
