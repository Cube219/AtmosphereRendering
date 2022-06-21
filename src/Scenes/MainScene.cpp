#include "MainScene.h"

#include "ResourceSystem/BaseMeshGenerator.h"
#include "GameObjectSystem/GameObject.h"
#include "GameObjectSystem/GameObjectSystem.h"

#include "RenderSystem/RenderSystem.h"

void MainScene::Begin()
{
    mBoxMesh = BaseMeshGenerator::GetBoxMesh();
    mPlaneMesh = BaseMeshGenerator::GetPlaneMesh();

    mTopBoxMaterial.diffuse = vec4(1, 0, 0, 1);
    mBottomBoxMaterial.diffuse = vec4(0, 1, 0, 1);
    mLeftBoxMaterial.diffuse = vec4(0, 0, 1, 1);
    mRightBoxMaterial.diffuse = vec4(1, 0, 0, 1);
    mPlaneMaterial.diffuse = vec4(0.5, 0.5, 0.5, 1);

    mTopBox = std::make_shared<Renderable>(mBoxMesh);
    mTopBox->SetPosition(vec3(0, 0, -5));
    mTopBox->GetRendererComponent()->SetMaterial(0, &mTopBoxMaterial);
    mTopBox->GetRendererComponent()->SetUseShading(false);
    GameObjectSystem::RegisterGameObject(mTopBox);

    mBottomBox = std::make_shared<Renderable>(mBoxMesh);
    mBottomBox->SetPosition(vec3(0, 0, 5));
    mBottomBox->GetRendererComponent()->SetMaterial(0, &mBottomBoxMaterial);
    mBottomBox->GetRendererComponent()->SetUseShading(false);
    GameObjectSystem::RegisterGameObject(mBottomBox);

    mLeftBox = std::make_shared<Renderable>(mBoxMesh);
    mLeftBox->SetPosition(vec3(-5, 0, 0));
    mLeftBox->GetRendererComponent()->SetMaterial(0, &mLeftBoxMaterial);
    mLeftBox->GetRendererComponent()->SetUseShading(false);
    GameObjectSystem::RegisterGameObject(mLeftBox);

    mRightBox = std::make_shared<Renderable>(mBoxMesh);
    mRightBox->SetPosition(vec3(5, 0, 0));
    mRightBox->GetRendererComponent()->SetMaterial(0, &mRightBoxMaterial);
    mRightBox->GetRendererComponent()->SetUseShading(false);
    GameObjectSystem::RegisterGameObject(mRightBox);

    mPlane = std::make_shared<Renderable>(mPlaneMesh);
    mPlane->SetScale(vec3(3, 1, 3));
    mPlane->SetPosition(vec3(0, 0, -5));
    mPlane->GetRendererComponent()->SetMaterial(0, &mPlaneMaterial);
    GameObjectSystem::RegisterGameObject(mPlane);

    RenderSystem::SetCameraInfo(0.7853f, 1.0f, 1000.0f);
    RenderSystem::SetCameraPosition(vec3(0, 0, 0));
    RenderSystem::SetCameraRotation(vec3(10, 0, 0));
}

void MainScene::End()
{
    GameObjectSystem::UnregisterGameObjectImmediate(mPlane);
    mPlane = nullptr;
    GameObjectSystem::UnregisterGameObjectImmediate(mRightBox);
    mRightBox = nullptr;
    GameObjectSystem::UnregisterGameObjectImmediate(mLeftBox);
    mLeftBox = nullptr;
    GameObjectSystem::UnregisterGameObjectImmediate(mBottomBox);
    mBottomBox = nullptr;
    GameObjectSystem::UnregisterGameObjectImmediate(mTopBox);
    mTopBox = nullptr;

    mPlaneMesh = nullptr;
    mBoxMesh = nullptr;
}

void MainScene::Update(double dt)
{
}

void MainScene::OnKeyPress(int code)
{
}
