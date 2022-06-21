#include "SceneSystem.h"

#include "../ResourceSystem/Texture.h"

SPtr<Scene> SceneSystem::mCurrentScene;
SPtr<Scene> SceneSystem::mNextScene;

SceneSystem::Status SceneSystem::mStatus;
float SceneSystem::mCurrentAlpha;
int SceneSystem::mLoadedDelayCount;

void SceneSystem::Initialize()
{
    mCurrentScene = nullptr;
    mNextScene = nullptr;
    mStatus = SceneSystem::Status::None;

    mLoadedDelayCount = 0;
}

void SceneSystem::Shutdown()
{
    if(mCurrentScene) {
        mCurrentScene->End();
        mCurrentScene = nullptr;
    }
}

void SceneSystem::Update(double dt)
{
    switch(mStatus) {
        case Status::Running:
            mCurrentScene->Update(dt);
            break;
        case Status::FadeOut:
            FadeOut(dt);
            break;
        case Status::FadeIn:
            FadeIn(dt);
            break;
        case Status::Loading:
            Loading();
            break;
        case Status::Loaded:
            Loaded();
            break;
    }
}

void SceneSystem::OnResize(int width, int height)
{
    if(mCurrentScene) {
        mCurrentScene->OnResize(width, height);
    }
}

void SceneSystem::OnKeyPress(int code)
{
    if(mCurrentScene) {
        mCurrentScene->OnKeyPress(code);
    }
}

void SceneSystem::OnKeyRelease(int code)
{
    if(mCurrentScene) {
        mCurrentScene->OnKeyRelease(code);
    }
}

void SceneSystem::FadeOut(double dt)
{
    mCurrentAlpha += 100.0f * float(dt);
    if(mCurrentAlpha >= 1.0f) {
        mCurrentAlpha = 1.0f;
        mStatus = Status::Loading;
    }

}

void SceneSystem::FadeIn(double dt)
{
    mCurrentAlpha -= 100.0f * float(dt);
    if(mCurrentAlpha <= 0.0f) {
        mCurrentAlpha = 0.0f;
        mStatus = Status::Running;
        Core::ResumeGame();
    }
}

void SceneSystem::Loading()
{
    if(mCurrentScene != nullptr) {
        mCurrentScene->End();
    }
    mNextScene->Begin();
    
    mCurrentScene = nullptr;
    mCurrentScene = mNextScene;
    mNextScene = nullptr;

    mStatus = Status::Loaded;
}

void SceneSystem::Loaded()
{
    mStatus = Status::FadeIn;
}
