#pragma once

#ifndef __SHADER_H__
#define __SHADER_H__

#include "BaseHeader.h"

class Shader
{
public:
    static SPtr<Shader> Load(const char* vertexPath, const char* fragmentPath);
    static SPtr<Shader> LoadCompute(const char* path);

private:
    static GLuint LoadShaderFromFile(const char* path, GLenum shaderType);
    static bool LinkProgram(GLuint program);

public:
    Shader(GLuint program, GLuint shader1, GLuint shader2);
    ~Shader();

    GLuint GetProgram() const { return mProgram; }

private:
    GLuint mShader1;
    GLuint mShader2;
    GLuint mProgram;
};

#endif // __SHADER_H__
