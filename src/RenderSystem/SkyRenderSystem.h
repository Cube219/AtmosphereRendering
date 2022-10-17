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

    static void SetSunValue(float _sun) { sun = _sun; }

private:
    static SPtr<Mesh> sphereMesh;
    static SPtr<Shader> skyShader;

    static float innerRadius;
    static float outerRadius;

    static float g;
    static float Kr;
    static float Km;
    static float sun;
    static vec3 waveLength4;
};

#endif // __SKY_RENDER_SYSTEM_H__
