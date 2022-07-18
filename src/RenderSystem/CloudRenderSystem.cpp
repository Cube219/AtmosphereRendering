#include "CloudRenderSystem.h"

#include "ResourceSystem/Shader.h"
#include "ResourceSystem/Mesh.h"
#include "ResourceSystem/BaseMeshGenerator.h"

GLuint CloudRenderSystem::shapeTexture3D;
GLuint CloudRenderSystem::detailTexture3D;
GLuint CloudRenderSystem::weatherTexture;

void CloudRenderSystem::Initialize()
{
}

void CloudRenderSystem::Shutdown()
{
}

void CloudRenderSystem::SubRender()
{
}

void CloudRenderSystem::InitTextures()
{
    glGenTextures(1, &shapeTexture3D);
    glBindTexture(GL_TEXTURE_3D, shapeTexture3D);
    glTexStorage3D(GL_TEXTURE_3D, 6, GL_RGBA8, 128, 128, 128);

    glGenTextures(1, &detailTexture3D);
    glBindTexture(GL_TEXTURE_3D, detailTexture3D);
    glTexStorage3D(GL_TEXTURE_3D, 6, GL_RGBA8, 32, 32, 32);
}
