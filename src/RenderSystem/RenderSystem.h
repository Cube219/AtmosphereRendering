#pragma once

#ifndef __RENDER_SYSTEM_H__
#define __RENDER_SYSTEM_H__

#include "BaseHeader.h"

class Mesh;
class RendererComponent;
class Shader;

struct Camera
{
    vec3 position;
    vec3 direction;

    float fov;
    float aspect;
    float nearV;
    float farV;
    mat4 viewMatrix;
    mat4 projMatrix;
    bool isDirty;
};

class RenderSystem
{
public:
    RenderSystem() = delete;
    ~RenderSystem() = delete;

    static void Initizlie();
    static void Shutdown();

    static void Render();
    static void OnResize(int width, int height);

    static void RegisterRendererComponent(SPtr<RendererComponent> c);
    static void UnregisterRendererComponent(SPtr<RendererComponent> c);

    static void SetCameraInfo(float fov, float nearV, float farV);
    static void SetCameraPosition(vec3 position);
    static void SetCameraDirection(vec3 rotation);
    static void UpdateCameraMatrix();
    static const Camera& GetCamera() { return mDefaultCamera; }

    static void SetLightDir(vec3 dir);
    static vec3 GetLightDir() { return mLightDir; }

    static GLuint GetMainFramebuffer() { return mMainFramebuffer; }
    static GLuint GetColorMap() { return mColorMap; }
    static GLuint GetDepthMap() { return mDepthMap; }

private:
    static void RecreateMaps();

    static int mWidth;
    static int mHeight;

    static bool mIsDirty;
    static Vector<SPtr<RendererComponent>> mComponents;
    
    static SPtr<Shader> mDefaultShader;
    static Camera mDefaultCamera;

    static vec3 mLightDir;

    static SPtr<Mesh> mPlaneMesh;
    static SPtr<Shader> mFinalShader;
    static GLuint mMainFramebuffer;
    static GLuint mColorMap;
    static GLuint mDepthMap;
};

#endif // __RENDER_SYSTEM_H__
