#include "RenderDebugSystem.h"

#include "Core.h"

#include "ResourceSystem/Shader.h"
#include "ResourceSystem/Mesh.h"
#include "ResourceSystem/BaseMeshGenerator.h"

SPtr<Shader> RenderDebugSystem::debugPlaneShader;
SPtr<Mesh> RenderDebugSystem::planeMesh;

void RenderDebugSystem::Initialize()
{
    debugPlaneShader = Shader::Load("..\\shaders\\debug.vert", "..\\shaders\\debug.frag");
    planeMesh = BaseMeshGenerator::GetPlaneMesh();
}

void RenderDebugSystem::Shutdown()
{
    planeMesh = nullptr;
}

void RenderDebugSystem::SubRender()
{
    glUseProgram(debugPlaneShader->GetProgram());
    glBindVertexArray(planeMesh->GetVertexArray());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeMesh->GetIndexBuffer());

    mat4 uiMatrix = GetUIMatrix(0);
    glUniformMatrix4fv(glGetUniformLocation(debugPlaneShader->GetProgram(), "ui_matrix"), 1, GL_FALSE, value_ptr(uiMatrix));

    glDrawElements(GL_TRIANGLES, planeMesh->GetIndicesNum(), GL_UNSIGNED_INT, 0);
}

mat4 RenderDebugSystem::GetUIMatrix(int idx)
{
    int windowWidth = Core::GetWindowWidth();
    int windowHeight = Core::GetWindowHeight();

    mat4 scaleMat = scale(vec3(200, 200, 1));
    mat4 posMat = translate(vec3((idx * 200) + 200 , 200, 0));
    mat4 toViewMat = scale(vec3(1.0f / windowWidth * 2.0f, 1.0f / windowHeight * 2.0f, 1));
    mat4 pivotMat = translate(vec3(-1, -1, 0));

    return pivotMat * toViewMat * posMat * scaleMat;
}
