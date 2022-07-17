#pragma once

#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "BaseHeader.h"

#include "RenderSystem/RendererComponent.h"
#include "SceneSystem/Scene.h"

class Mesh;
class GameObject;
class Renderable;

class MainScene : public Scene
{
public:
    MainScene() {}
    virtual ~MainScene() {}

    virtual void Begin() override;
    virtual void End() override;

    virtual void Update(double dt) override;
    virtual void OnKeyPress(int code) override;

private:
    SPtr<Mesh> mBoxMesh;
    SPtr<Mesh> mPlaneMesh;

    Material mTopBoxMaterial;
    Material mBottomBoxMaterial;
    Material mLeftBoxMaterial;
    Material mRightBoxMaterial;
    Material mPlaneMaterial;

    SPtr<Renderable> mTopBox;
    SPtr<Renderable> mBottomBox;
    SPtr<Renderable> mLeftBox;
    SPtr<Renderable> mRightBox;
    SPtr<Renderable> mPlane;

    vec3 mPlayerPosition;
    float mPlayerYaw;
    float mPlayerPitch;

    vec2 mLastMousePos;
};

#endif // __MAIN_SCENE_H__
