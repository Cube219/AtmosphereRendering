#pragma once

#ifndef __CLOUD_RENDER_SYSTEM_H__
#define __CLOUD_RENDER_SYSTEM_H__

#include "BaseHeader.h"

class Shader;
class Mesh;

class CloudRenderSystem
{
public:
    CloudRenderSystem() = delete;
    ~CloudRenderSystem() = delete;

    static void Initialize();
    static void Shutdown();

    static void SubRender();

private:
    static void InitTextures();
    static void DestroyTextures();

    static GLuint shapeTexture3D;
    static GLuint detailTexture3D;
    static GLuint weatherTexture;

    static SPtr<Mesh> planeMesh;
    static SPtr<Shader> cloudShader;
};

#endif // __CLOUD_RENDER_SYSTEM_H__
