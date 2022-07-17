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

private:
    static int mWidth;
    static int mHeight;

    static bool mIsDirty;
    static Vector<SPtr<RendererComponent>> mComponents;
    
    static SPtr<Shader> mDefaultShader;
    static Camera mDefaultCamera;
};

#endif // __RENDER_SYSTEM_H__
