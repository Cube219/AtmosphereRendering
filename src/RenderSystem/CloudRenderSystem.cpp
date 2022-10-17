#include "CloudRenderSystem.h"

#include "Core.h"
#include "RenderSystem.h"
#include "RenderDebugSystem.h"
#include "ResourceSystem/Shader.h"
#include "ResourceSystem/Mesh.h"
#include "ResourceSystem/BaseMeshGenerator.h"

GLuint CloudRenderSystem::shapeTexture3D;
GLuint CloudRenderSystem::detailTexture3D;
GLuint CloudRenderSystem::weatherTexture;

SPtr<Mesh> CloudRenderSystem::planeMesh;
SPtr<Shader> CloudRenderSystem::cloudShader;

SPtr<Shader> CloudRenderSystem::shapeGenShader;
SPtr<Shader> CloudRenderSystem::detailGenShader;
SPtr<Shader> CloudRenderSystem::weatherGenShader;

float CloudRenderSystem::innerSphereRadius = 20000000.0f;
float CloudRenderSystem::outerSphereRadius = 21800000.0f;
float CloudRenderSystem::sphereCenterY = -18000000.0f;

float CloudRenderSystem::heightFactor = 0.0f;

void CloudRenderSystem::Initialize()
{
    InitTextures();

    planeMesh = BaseMeshGenerator::GetPlaneMesh();
    cloudShader = Shader::Load("..\\shaders\\cloud.vert", "..\\shaders\\cloud.frag");

    // RenderDebugSystem::SetDebugTexture(weatherTexture);
}

void CloudRenderSystem::Shutdown()
{
    cloudShader = nullptr;
    planeMesh = nullptr;

    DestroyTextures();
}

void CloudRenderSystem::SubRender()
{
    glDisable(GL_DEPTH_TEST);

    // TODO: 별도의 Framebuffer에 rendering
    GLuint program = cloudShader->GetProgram();
    glUseProgram(program);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, RenderSystem::GetDepthMap());
    glUniform1i(glGetUniformLocation(program, "depthMap"), 0);

    const Camera& camera = RenderSystem::GetCamera();
    float size[2] = { (float)Core::GetWindowWidth(), (float)Core::GetWindowHeight() };
    glUniform2fv(glGetUniformLocation(program, "screenSize"), 1, size);
    glUniform1f(glGetUniformLocation(program, "FOV"), camera.fov);
    glUniform3f(glGetUniformLocation(program, "camPos"), camera.position.x, camera.position.y, camera.position.z);

    mat4 projView = camera.projMatrix * camera.viewMatrix;
    glUniformMatrix4fv(glGetUniformLocation(program, "projView"), 1, GL_FALSE, value_ptr(projView));
    mat4 invView = inverse(camera.viewMatrix);
    glUniformMatrix4fv(glGetUniformLocation(program, "invView"), 1, GL_FALSE, value_ptr(invView));

    glUniform3f(glGetUniformLocation(program, "sphereCenter"), camera.position.x, sphereCenterY + 100 * heightFactor, camera.position.z);
    glUniform1f(glGetUniformLocation(program, "innerSphereRadius"), innerSphereRadius);
    glUniform1f(glGetUniformLocation(program, "outerSphereRadius"), outerSphereRadius);

    vec3 lightDir = RenderSystem::GetLightDir();
    glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_3D, shapeTexture3D);
    glUniform1i(glGetUniformLocation(program, "shapeTex"), 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_3D, detailTexture3D);
    glUniform1i(glGetUniformLocation(program, "detailTex"), 2);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, weatherTexture);
    glUniform1i(glGetUniformLocation(program, "weatherMap"), 3);

    // Draw
    glBindVertexArray(planeMesh->GetVertexArray());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeMesh->GetIndexBuffer());
    glDrawElements(GL_TRIANGLES, planeMesh->GetIndicesNum(), GL_UNSIGNED_INT, 0);

    glEnable(GL_DEPTH_TEST);
}

void CloudRenderSystem::InitTextures()
{
    GLuint program;

    // Shape texture
    glGenTextures(1, &shapeTexture3D);
    glBindTexture(GL_TEXTURE_3D, shapeTexture3D);
    glTexStorage3D(GL_TEXTURE_3D, 6, GL_RGBA8, 128, 128, 128);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    shapeGenShader = Shader::LoadCompute("..\\shaders\\generator\\perlinworley.comp");
    program = shapeGenShader->GetProgram();
    glUseProgram(program);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, shapeTexture3D);
    glBindImageTexture(0, shapeTexture3D, 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA8);
    glUniform1i(glGetUniformLocation(program, "outVolTex"), 0);

    glDispatchCompute(128, 128, 128);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    // Detail texture
    glGenTextures(1, &detailTexture3D);
    glBindTexture(GL_TEXTURE_3D, detailTexture3D);
    glTexStorage3D(GL_TEXTURE_3D, 6, GL_RGBA8, 32, 32, 32);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    detailGenShader = Shader::LoadCompute("..\\shaders\\generator\\worley.comp");
    program = detailGenShader->GetProgram();
    glUseProgram(program);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, detailTexture3D);
    glBindImageTexture(0, detailTexture3D, 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA8);
    glUniform1i(glGetUniformLocation(program, "outVolTex"), 0);

    glDispatchCompute(32, 32, 32);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    // Weather texture
    glGenTextures(1, &weatherTexture);
    glBindTexture(GL_TEXTURE_2D, weatherTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 2048, 2048, 0, GL_RGBA, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    weatherGenShader = Shader::LoadCompute("..\\shaders\\generator\\weather.comp");
    program = weatherGenShader->GetProgram();
    glUseProgram(program);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, weatherTexture);
    glBindImageTexture(0, weatherTexture, 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA8);
    glUniform1i(glGetUniformLocation(program, "outWeatherTex"), 0);

    glMemoryBarrier(GL_ALL_BARRIER_BITS);
    glDispatchCompute(2048, 2048, 1);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);
}

void CloudRenderSystem::DestroyTextures()
{
    glDeleteTextures(1, &shapeTexture3D);
    glDeleteTextures(1, &detailTexture3D);
    glDeleteTextures(1, &weatherTexture);

    weatherGenShader = nullptr;
    detailGenShader = nullptr;
    shapeGenShader = nullptr;
}
