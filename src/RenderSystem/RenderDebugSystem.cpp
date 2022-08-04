#include "RenderDebugSystem.h"

#include "Core.h"

#include "ResourceSystem/Shader.h"
#include "ResourceSystem/Mesh.h"
#include "ResourceSystem/BaseMeshGenerator.h"

SPtr<Shader> RenderDebugSystem::debugPlaneShader;
SPtr<Mesh> RenderDebugSystem::planeMesh;

GLuint RenderDebugSystem::texture = 0;

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
    if(texture == 0) return;

    GLuint program = debugPlaneShader->GetProgram();

    glUseProgram(program);
    glBindVertexArray(planeMesh->GetVertexArray());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeMesh->GetIndexBuffer());

    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, texture);
    // glUniform1i(glGetUniformLocation(program, "debugTex"), 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, texture);
    glUniform1i(glGetUniformLocation(program, "debugTex"), 0);

    for(int i = 0; i < 4; ++i) {
        mat4 uiMatrix = GetUIMatrix(i);
        glUniformMatrix4fv(glGetUniformLocation(program, "ui_matrix"), 1, GL_FALSE, value_ptr(uiMatrix));
        glUniform1i(glGetUniformLocation(program, "channel"), i);

        glDrawElements(GL_TRIANGLES, planeMesh->GetIndicesNum(), GL_UNSIGNED_INT, 0);
    }
}

mat4 RenderDebugSystem::GetUIMatrix(int idx)
{
    int windowWidth = Core::GetWindowWidth();
    int windowHeight = Core::GetWindowHeight();

    mat4 scaleMat = scale(vec3(150, 150, 1));
    mat4 posMat = translate(vec3((idx * 300) + 150 , 150, 0));
    mat4 toViewMat = scale(vec3(1.0f / windowWidth * 2.0f, 1.0f / windowHeight * 2.0f, 1));
    mat4 pivotMat = translate(vec3(-1, -1, 0));

    return pivotMat * toViewMat * posMat * scaleMat;
}
