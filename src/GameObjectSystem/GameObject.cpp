#include "GameObject.h"

#include "../RenderSystem/RendererComponent.h"
#include "../RenderSystem/RenderSystem.h"
#include "../ResourceSystem/Mesh.h"

GameObject::GameObject(GameObjectType type) :
    mIsActivated(true),
    mType(type),
    mPosition(0.0f, 0.0f, 0.0f),
    mRotation(0.0f, 0.0f, 0.0f),
    mScale(1.0f, 1.0f, 1.0f),
    mIsDirty(true),
    mForward(0, 0, -1),
    mUp(0, 1, 0),
    mRight(1, 0, 0)
{
}

GameObject::~GameObject()
{
}

void GameObject::PreUpdate()
{
    if(mIsDirty) {
        UpdateModelMatrix();
        mIsDirty = false;
    }
}

void GameObject::Update(double dt)
{
    if(mIsActivated == true) {
        OnUpdate(dt);
    }
}

void GameObject::PostUpdate()
{
    if(mIsDirty) {
        UpdateModelMatrix();
        mIsDirty = false;
    }
}

void GameObject::UpdateModelMatrix()
{
    mat4 positionMatrix =
    {
        1, 0, 0, mPosition.x,
        0, 1, 0, mPosition.y,
        0, 0, 1, mPosition.z,
        0, 0, 0, 1
    };
    float rotX = radians(mRotation.x);
    float rotY = radians(mRotation.y + 180.0f);
    float rotZ = radians(mRotation.z);
    mat4 rotationMatrix = rotate(rotX, vec3(1, 0, 0));
    rotationMatrix *= rotate(rotY, vec3(0, 1, 0));
    rotationMatrix *= rotate(rotZ, vec3(0, 0, 1));

    mat4 scaleMatrix =
    {
        mScale.x, 0, 0, 0,
        0, mScale.y, 0, 0,
        0, 0, mScale.z, 0,
        0, 0, 0, 1
    };

    mModelMatrix = positionMatrix * rotationMatrix * scaleMatrix;

    vec4 forward = vec4(0, 0, 1, 0);
    vec4 up = vec4(0, 1, 0, 0);
    vec4 right = vec4(1, 0, 0, 0);

    forward = rotationMatrix * forward;
    up = rotationMatrix * up;
    right = rotationMatrix * right;

    mForward = vec3(forward.x, forward.y, forward.z);
    mUp = vec3(up.x, up.y, up.z);
    mRight = vec3(right.x, right.y, right.z);
}

Renderable::Renderable(SPtr<Mesh> mesh)
{
    mRendererComponent = std::make_shared<RendererComponent>(*this);
    RenderSystem::RegisterRendererComponent(mRendererComponent);
    mRendererComponent->SetMesh(mesh);
}

Renderable::~Renderable()
{
    RenderSystem::UnregisterRendererComponent(mRendererComponent);
    mRendererComponent = nullptr;
}
