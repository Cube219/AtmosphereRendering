#include "MainScene.h"

#include "Core.h"

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
    mRightBoxMaterial.diffuse = vec4(1, 1, 1, 1);
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
    mPlane->SetScale(vec3(40, 1, 40));
    mPlane->SetPosition(vec3(0, 0, -1));
    mPlane->SetRotation(vec3(90, 0, 0));
    mPlane->GetRendererComponent()->SetMaterial(0, &mPlaneMaterial);
    GameObjectSystem::RegisterGameObject(mPlane);

    mPlayerPosition = vec3(0, 0, 0);
    mPlayerYaw = 0;
    mPlayerPitch = 0;

    RenderSystem::SetCameraInfo(0.7853f, 1.0f, 100.0f);
    RenderSystem::SetCameraPosition(vec3(0, 0, -1));
    RenderSystem::SetCameraDirection(vec3(0, 0, 0));
    mLastMousePos = Core::GetMousePosition();
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
    vec2 currentMousePos = Core::GetMousePosition();

    if(Core::IsMousePressed(MouseButton::Left)) {
        mPlayerYaw -= (currentMousePos.x - mLastMousePos.x) * 0.1f;
        mPlayerPitch -= (currentMousePos.y - mLastMousePos.y) * 0.1f;
    }

    float yawRad = radians(mPlayerYaw);
    float pitchRad = radians(mPlayerPitch);

    vec3 direction;
    direction.x = sin(yawRad) * cos(pitchRad);
    direction.y = sin(pitchRad);
    direction.z = cos(yawRad) * cos(pitchRad);
    direction = normalize(direction);

    float dx = 0, dy = 0, dz = 0;
    if(Core::IsKeyPressed('A')) {
        dx -= 0.1f;
    }
    if(Core::IsKeyPressed('D')) {
        dx += 0.1f;
    }
    if(Core::IsKeyPressed('W')) {
        dz += 0.1f;
    }
    if(Core::IsKeyPressed('S')) {
        dz -= 0.1f;
    }
    if(Core::IsKeyPressed('Q')) {
        dy -= 0.1f;
    }
    if(Core::IsKeyPressed('E')) {
        dy += 0.1f;
    }
    vec3 forward = direction;
    vec3 up = vec3(0, 1, 0);
    vec3 right = normalize(cross(forward, up));
    forward *= dz;
    up *= dy;
    right *= dx;

    mPlayerPosition += forward + up + right;

    RenderSystem::SetCameraPosition(mPlayerPosition);
    RenderSystem::SetCameraDirection(direction);

    mLastMousePos = currentMousePos;
}

void MainScene::OnKeyPress(int code)
{
}
