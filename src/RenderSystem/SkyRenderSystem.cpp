#include "SkyRenderSystem.h"

#include "Core.h"
#include "RenderSystem.h"
#include "RenderDebugSystem.h"
#include "ResourceSystem/Shader.h"
#include "ResourceSystem/Mesh.h"
#include "ResourceSystem/BaseMeshGenerator.h"

SPtr<Mesh> SkyRenderSystem::sphereMesh;
SPtr<Shader> SkyRenderSystem::skyShader;

float SkyRenderSystem::innerRadius;
float SkyRenderSystem::outerRadius;

float SkyRenderSystem::g;
float SkyRenderSystem::Kr;
float SkyRenderSystem::Km;
float SkyRenderSystem::sun;
vec3 SkyRenderSystem::waveLength4;

void SkyRenderSystem::Initialize()
{
    sphereMesh = BaseMeshGenerator::GetSphereMesh();
    skyShader = Shader::Load("..\\shaders\\sky.vert", "..\\shaders\\sky.frag");

    innerRadius = 10.0f;
    outerRadius = 12.25f;

    g = -0.95f;
    Kr = 0.0025f;
    Km = 0.001f;
    sun = 50.0f;
    waveLength4.x = powf(0.650f, 4.0f);
    waveLength4.y = powf(0.570f, 4.0f);
    waveLength4.z = powf(0.475f, 4.0f);
}

void SkyRenderSystem::Shutdown()
{
    skyShader = nullptr;
    sphereMesh = nullptr;    
}

void SkyRenderSystem::SubRender()
{
    glDisable(GL_DEPTH_TEST);
    glFrontFace(GL_CW); // It draw inside of sphere, so reverse front face

    GLuint program = skyShader->GetProgram();
    glUseProgram(program);

    const Camera& camera = RenderSystem::GetCamera();
    mat4 model =
    {
        1, 0, 0, camera.position.x,
        0, 1, 0, camera.position.y - 10.05f,
        0, 0, 1, camera.position.z,
        0, 0, 0, 1
    };
    model = transpose(model) * scale(vec3(outerRadius));
    mat4 mvp = camera.projMatrix * camera.viewMatrix * model;
    glUniformMatrix4fv(glGetUniformLocation(program, "MVP"), 1, GL_FALSE, value_ptr(mvp));

    glUniform3f(glGetUniformLocation(program, "cameraPos"), 0, 10.0f, 0);
    vec3 lightDir = normalize(RenderSystem::GetLightDir());
    glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);
    glUniform1f(glGetUniformLocation(program, "g"), g);
    glUniform3f(glGetUniformLocation(program, "invWaveLength4"), 1.0f / waveLength4.x, 1.0f / waveLength4.y, 1.0f / waveLength4.z);
    
    glUniform1f(glGetUniformLocation(program, "innerRadius"), innerRadius);
    glUniform1f(glGetUniformLocation(program, "outerRadius"), outerRadius);

    glUniform1f(glGetUniformLocation(program, "Kr"), Kr);
    glUniform1f(glGetUniformLocation(program, "Km"), Km);
    glUniform1f(glGetUniformLocation(program, "sun"), sun);
    glUniform1f(glGetUniformLocation(program, "Scale"), 1.0f / (outerRadius - innerRadius));

    // Draw
    glBindVertexArray(sphereMesh->GetVertexArray());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereMesh->GetIndexBuffer());
    glDrawElements(GL_TRIANGLES, sphereMesh->GetIndicesNum(), GL_UNSIGNED_INT, 0);

    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);
}
