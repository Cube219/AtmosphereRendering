#include "RenderSystem.h"

#include <algorithm>

#include "RendererComponent.h"
#include "../Core.h"
#include "../ResourceSystem/Mesh.h"
#include "../ResourceSystem/Shader.h"
#include "../ResourceSystem/Texture.h"

int RenderSystem::mWidth;
int RenderSystem::mHeight;

bool RenderSystem::mIsDirty;
Vector<SPtr<RendererComponent>> RenderSystem::mComponents;

SPtr<Shader> RenderSystem::mDefaultShader;
Camera RenderSystem::mDefaultCamera;

void RenderSystem::Initizlie()
{
    glLineWidth(1.0f);
    glClearColor(39 / 255.0f, 40 / 255.0f, 34 / 255.0f, 1.0f);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSwapInterval(1);

    mIsDirty = false;
    mDefaultShader = Shader::Load("..\\shaders\\default.vert", "..\\shaders\\default.frag");

    Vector<Vertex> vertices;
    Vector<uint> indices;

    Vertex v;
    v.pos.x = 0;
    vertices.push_back(v);
    v.pos.x = 1;
    vertices.push_back(v);
    v.pos.x = 2;
    vertices.push_back(v);
    v.pos.x = 3;
    vertices.push_back(v);
    indices = { 0,1,2, 2,1,3 };
}

void RenderSystem::Shutdown()
{
    mDefaultShader = nullptr;
    mComponents.clear();
}

void RenderSystem::Render()
{
    if(mIsDirty == true) {
        std::sort(mComponents.begin(), mComponents.end(), [](const SPtr<RendererComponent>& l, const SPtr<RendererComponent>& r) {
            return l->GetMesh() < r->GetMesh();
        });
        mIsDirty = false;
    }

    if(mDefaultCamera.isDirty == true) {
        UpdateCameraMatrix();
        mDefaultCamera.isDirty = false;
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glCullFace(GL_BACK);

    // Darw objects
    glViewport(0, 0, mWidth, mHeight);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(mDefaultShader->GetProgram());

    // Set view/projMatrix
    GLint uloc;
    uloc = glGetUniformLocation(mDefaultShader->GetProgram(), "view_matrix");
    glUniformMatrix4fv(uloc, 1, GL_FALSE, value_ptr(mDefaultCamera.viewMatrix));
    uloc = glGetUniformLocation(mDefaultShader->GetProgram(), "projection_matrix");
    glUniformMatrix4fv(uloc, 1, GL_FALSE, value_ptr(mDefaultCamera.projMatrix));

    // Set lights
    glUniform3fv(glGetUniformLocation(mDefaultShader->GetProgram(), "cameraPos"), 1, value_ptr(mDefaultCamera.position));

    // Draws
    SPtr<Mesh> currentMesh = nullptr;
    for(auto c : mComponents) {
        if(c->GetIsVisible() == false) continue;

        if(currentMesh != c->GetMesh()) {
            currentMesh = c->GetMesh();
            glBindVertexArray(currentMesh->GetVertexArray());
        }

        uloc = glGetUniformLocation(mDefaultShader->GetProgram(), "model_matrix");
        glUniformMatrix4fv(uloc, 1, GL_TRUE, value_ptr(c->GetModelMatrix()));

        uloc = glGetUniformLocation(mDefaultShader->GetProgram(), "debug_color");
        glUniform4fv(uloc, 1, value_ptr(c->GetDebugColor()));

        uloc = glGetUniformLocation(mDefaultShader->GetProgram(), "useShading");
        glUniform1i(uloc, c->GetUseShading());

        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, currentMesh->GetIndexBuffer());
        auto& subMeshes = currentMesh->GetSubMeshes();
        auto& materials = c->GetMaterials();
        for(uint i = 0; i < subMeshes.size(); i++) {
            const SubMesh& sm = subMeshes[i];
            const Material* mat = materials[i];

            if(mat != nullptr) {
                if(mat->tDiffuse != nullptr) {
                    glUniform1i(glGetUniformLocation(mDefaultShader->GetProgram(), "useDiffuseTexture"), true);
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, mat->tDiffuse->GetTextureObject());
                    glUniform1i(glGetUniformLocation(mDefaultShader->GetProgram(), "diffuseTex"), 0);
                } else {
                    glUniform1i(glGetUniformLocation(mDefaultShader->GetProgram(), "useDiffuseTexture"), false);
                    glUniform4f(glGetUniformLocation(mDefaultShader->GetProgram(), "diffuse"), mat->diffuse.r, mat->diffuse.g, mat->diffuse.b, mat->diffuse.a);
                }

                if(mat->tSpecular != nullptr) {
                    glUniform1i(glGetUniformLocation(mDefaultShader->GetProgram(), "useSpecularTexture"), true);
                    glActiveTexture(GL_TEXTURE1);
                    glBindTexture(GL_TEXTURE_2D, mat->tSpecular->GetTextureObject());
                    glUniform1i(glGetUniformLocation(mDefaultShader->GetProgram(), "specularTex"), 1);
                } else {
                    glUniform1i(glGetUniformLocation(mDefaultShader->GetProgram(), "useSpecularTexture"), false);
                    glUniform4f(glGetUniformLocation(mDefaultShader->GetProgram(), "specular"), mat->specular.r, mat->specular.g, mat->specular.b, mat->specular.a);
                }

                if(mat->tNormal != nullptr) {
                    glUniform1i(glGetUniformLocation(mDefaultShader->GetProgram(), "useNormalTexture"), true);
                    glActiveTexture(GL_TEXTURE2);
                    glBindTexture(GL_TEXTURE_2D, mat->tNormal->GetTextureObject());
                    glUniform1i(glGetUniformLocation(mDefaultShader->GetProgram(), "normalTex"), 2);
                } else {
                    glUniform1i(glGetUniformLocation(mDefaultShader->GetProgram(), "useNormalTexture"), false);
                }
            } else {
                glUniform1i(glGetUniformLocation(mDefaultShader->GetProgram(), "useDiffuseTexture"), false);
                glUniform4fv(glGetUniformLocation(mDefaultShader->GetProgram(), "diffuse"), 1, value_ptr(c->GetDebugColor()));
                glUniform1i(glGetUniformLocation(mDefaultShader->GetProgram(), "useSpecularTexture"), false);
                glUniform4f(glGetUniformLocation(mDefaultShader->GetProgram(), "specular"), 0, 0, 0, 0);
                glUniform1i(glGetUniformLocation(mDefaultShader->GetProgram(), "useNormalTexture"), false);
            }

            glDrawElements(GL_TRIANGLES, sm.indexCount, GL_UNSIGNED_INT, (GLvoid*)(sm.indexOffset * sizeof(GLuint)));
        }
    }

    glfwSwapBuffers(Core::GetGLFWwindow());
}

void RenderSystem::OnResize(int width, int height)
{
    mWidth = width;
    mHeight = height;

    mDefaultCamera.aspect = width / float(height);
    mDefaultCamera.isDirty = true;
}

void RenderSystem::RegisterRendererComponent(SPtr<RendererComponent> c)
{
    mIsDirty = true;

    mComponents.push_back(c);
}

void RenderSystem::UnregisterRendererComponent(SPtr<RendererComponent> c)
{
    auto findIt = std::find(mComponents.begin(), mComponents.end(), c);
    if(findIt != mComponents.end()) {
        mComponents.erase(findIt);
        mIsDirty = true;
    }
}

void RenderSystem::SetCameraInfo(float fov, float nearV, float farV)
{
    mDefaultCamera.fov = fov;
    mDefaultCamera.nearV = nearV;
    mDefaultCamera.farV = farV;
    mDefaultCamera.isDirty = true;
}

void RenderSystem::SetCameraPosition(vec3 position)
{
    mDefaultCamera.position = position;
    mDefaultCamera.isDirty = true;
}

void RenderSystem::SetCameraDirection(vec3 rotation)
{
    mDefaultCamera.direction = rotation;
    mDefaultCamera.isDirty = true;
}

void RenderSystem::UpdateCameraMatrix()
{
    mat4 viewMatrix = glm::lookAt(mDefaultCamera.position, mDefaultCamera.position + mDefaultCamera.direction, vec3(0, 1, 0));
    mat4 projectionMatrix = glm::perspective(mDefaultCamera.fov, mDefaultCamera.aspect, mDefaultCamera.nearV, mDefaultCamera.farV);
    mDefaultCamera.viewMatrix = viewMatrix;
    mDefaultCamera.projMatrix = projectionMatrix;
}
