#include "CloudRenderSystem.h"

#include "RenderDebugSystem.h"
#include "ResourceSystem/Shader.h"
#include "ResourceSystem/Mesh.h"
#include "ResourceSystem/BaseMeshGenerator.h"

GLuint CloudRenderSystem::shapeTexture3D;
GLuint CloudRenderSystem::detailTexture3D;
GLuint CloudRenderSystem::weatherTexture;

void CloudRenderSystem::Initialize()
{
    InitTextures();

    RenderDebugSystem::SetDebugTexture(weatherTexture);
}

void CloudRenderSystem::Shutdown()
{
}

void CloudRenderSystem::SubRender()
{
}

void CloudRenderSystem::InitTextures()
{
    SPtr<Shader> computeShader;
    GLuint program;

    // Shape texture
    // glGenTextures(1, &shapeTexture3D);
    // glBindTexture(GL_TEXTURE_3D, shapeTexture3D);
    // glTexStorage3D(GL_TEXTURE_3D, 6, GL_RGBA8, 128, 128, 128);
    //
    // Detail texture
    // glGenTextures(1, &detailTexture3D);
    // glBindTexture(GL_TEXTURE_3D, detailTexture3D);
    // glTexStorage3D(GL_TEXTURE_3D, 6, GL_RGBA8, 32, 32, 32);

    // Weather texture
    glGenTextures(1, &weatherTexture);
    glBindTexture(GL_TEXTURE_2D, weatherTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 2048, 2048, 0, GL_RGBA, GL_FLOAT, 0);

    computeShader = Shader::LoadCompute("..\\shaders\\generator\\weather.comp");
    program = computeShader->GetProgram();
    glUseProgram(program);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, weatherTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindImageTexture(0, weatherTexture, 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA8);
    glUniform1i(glGetUniformLocation(program, "outWeatherTex"), 0);

    glDispatchCompute(2048, 2048, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}
