#pragma once

#ifndef __SKY_RENDER_SYSTEM_H__
#define __SKY_RENDER_SYSTEM_H__

#include "BaseHeader.h"

class Shader;
class Mesh;

class SkyRenderSystem
{
public:
    SkyRenderSystem() = delete;
    ~SkyRenderSystem() = delete;

    static void Initialize();
    static void Shutdown();

    static void SubRender();

private:
    static SPtr<Mesh> planeMesh;
    static SPtr<Shader> skyShader;
};

#endif // __SKY_RENDER_SYSTEM_H__
