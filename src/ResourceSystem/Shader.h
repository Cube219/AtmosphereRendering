#pragma once

#ifndef __SHADER_H__
#define __SHADER_H__

#include "BaseHeader.h"

class Shader
{
public:
    static SPtr<Shader> Load(const char* vertexPath, const char* fragmentPath);

private:
    static GLuint LoadShaderFromFile(const char* path, GLenum shaderType);

public:
    Shader(GLuint program);
    ~Shader();

    GLuint GetProgram() const { return mProgram; }

private:
    GLuint mProgram;
};

#endif // __SHADER_H__
