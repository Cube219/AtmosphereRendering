#include "SkyRenderSystem.h"

#include "Core.h"
#include "RenderSystem.h"
#include "RenderDebugSystem.h"
#include "ResourceSystem/Shader.h"
#include "ResourceSystem/Mesh.h"
#include "ResourceSystem/BaseMeshGenerator.h"

SPtr<Mesh> SkyRenderSystem::planeMesh;
SPtr<Shader> SkyRenderSystem::skyShader;

void SkyRenderSystem::Initialize()
{
    planeMesh = BaseMeshGenerator::GetPlaneMesh();
    skyShader = Shader::Load("..\\shaders\\sky.vert", "..\\shaders\\sky.frag");
}

void SkyRenderSystem::Shutdown()
{
    skyShader = nullptr;
    planeMesh = nullptr;
}

void SkyRenderSystem::SubRender()
{
    glDisable(GL_DEPTH_TEST);

    GLuint program = skyShader->GetProgram();
    glUseProgram(program);

    // Draw
    glBindVertexArray(planeMesh->GetVertexArray());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeMesh->GetIndexBuffer());
    glDrawElements(GL_TRIANGLES, planeMesh->GetIndicesNum(), GL_UNSIGNED_INT, 0);

    glEnable(GL_DEPTH_TEST);
}
