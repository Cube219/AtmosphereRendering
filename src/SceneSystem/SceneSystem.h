#pragma once

#ifndef __SCENE_SYSTEM_H__
#define __SCENE_SYSTEM_H__

#include "BaseHeader.h"

#include "Scene.h"
#include "Core.h"

class SceneSystem
{
public:
    SceneSystem() = delete;
    ~SceneSystem() = delete;

    static void Initialize();
    static void Shutdown();

    static void Update(double dt);

    static void OnResize(int width, int height);
    static void OnKeyPress(int code);
    static void OnKeyRelease(int code);

    template <typename T>
    static void ChangeScene()
    {
        if(mStatus != Status::Running && mStatus != Status::None) return;

        mNextScene = std::make_shared<T>();
        
        mCurrentAlpha = 0.0f;
        mStatus = Status::FadeOut;
        Core::PauseGame();
    }

private:
    enum class Status
    {
        None, Running, FadeOut, FadeIn, Loading, Loaded
    };

    static void FadeOut(double dt);
    static void FadeIn(double dt);
    static void Loading();
    static void Loaded();

    static SPtr<Scene> mCurrentScene;
    static SPtr<Scene> mNextScene;

    static Status mStatus;
    static float mCurrentAlpha;
    static int mLoadedDelayCount;
};

#endif // __SCENE_SYSTEM_H__
