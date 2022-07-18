#pragma once

#ifndef __RENDER_DEBUG_SYSTEM_H__
#define __RENDER_DEBUG_SYSTEM_H__

#include "BaseHeader.h"

class Shader;
class Mesh;

class RenderDebugSystem
{
public:
    RenderDebugSystem() = delete;
    ~RenderDebugSystem() = delete;

    static void Initialize();
    static void Shutdown();

    static void SubRender();

private:
    static mat4 GetUIMatrix(int idx);

    static SPtr<Shader> debugPlaneShader;
    static SPtr<Mesh> planeMesh;
};

#endif // __RENDER_DEBUG_SYSTEM_H__
