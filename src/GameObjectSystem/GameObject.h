#pragma once

#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "BaseHeader.h"

class Mesh;
class RendererComponent;

enum class GameObjectType
{
    Player, Enemy, Obstacle, Other
};

class GameObject
{
public:
    GameObject(GameObjectType type = GameObjectType::Other);
    virtual ~GameObject();

    virtual void OnUpdate(double dt) {}

    GameObjectType GetType() const { return mType; }

    const mat4& GetModelMatrix() const
    {
        return mModelMatrix;
    }

    vec3 GetPosition() const
    {
        return mPosition;
    }
    vec3 GetRotation() const
    {
        return mRotation;
    }
    vec3 GetScale() const
    {
        return mScale;
    }
    vec4 GetDebugColor() const
    {
        return mDebugColor;
    }

    vec3 GetForward() const { return mForward; }
    vec3 GetUp() const { return mUp; }
    vec3 GetRight() const { return mRight; }

    void SetActivate(bool isActivated)
    {
        mIsActivated = isActivated;
    }

    void SetType(GameObjectType type)
    {
        mType = type;
    }

    void SetPosition(vec3 pos)
    {
        mPosition = pos;
        mIsDirty = true;
    }
    void SetRotation(vec3 rotation)
    {
        mRotation = rotation;
        mIsDirty = true;
    }
    void SetScale(vec3 scale)
    {
        mScale = scale;
        mIsDirty = true;
    }
    void SetDebugColor(vec4 color)
    {
        mDebugColor = color;
    }

private:
    friend class GameObjectSystem;

    void PreUpdate();
    void Update(double dt);
    void PostUpdate();

    void UpdateModelMatrix();

    bool mIsActivated;
    GameObjectType mType;

    bool mIsDirty;
    vec3 mPosition;
    vec3 mRotation;
    vec3 mScale;
    vec4 mDebugColor;

    mat4 mModelMatrix;

    vec3 mForward;
    vec3 mUp;
    vec3 mRight;
};

class Renderable : public GameObject
{
public:
    Renderable(SPtr<Mesh> mesh);
    virtual ~Renderable();

    SPtr<RendererComponent> GetRendererComponent() const { return mRendererComponent; }

protected:
    SPtr<RendererComponent> mRendererComponent;
};

#endif // __GAMEOBJECT_H__
