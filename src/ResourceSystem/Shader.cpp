#include "Shader.h"

#include <fstream>
#include <sstream>

SPtr<Shader> Shader::Load(const char* vertexPath, const char* fragmentPath)
{
    GLuint program = glCreateProgram();
    glUseProgram(program);

    GLuint vertexShader = LoadShaderFromFile(vertexPath, GL_VERTEX_SHADER);
    GLuint fragmentShader = LoadShaderFromFile(fragmentPath, GL_FRAGMENT_SHADER);

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    if(!LinkProgram(program)) return nullptr;

    return std::make_shared<Shader>(program, vertexShader, fragmentShader);
}

SPtr<Shader> Shader::LoadCompute(const char* path)
{
    GLuint program = glCreateProgram();
    glUseProgram(program);

    GLuint computeShader = LoadShaderFromFile(path, GL_COMPUTE_SHADER);

    glAttachShader(program, computeShader);

    if(!LinkProgram(program)) return nullptr;

    return std::make_shared<Shader>(program, computeShader, 0);
}

GLuint Shader::LoadShaderFromFile(const char* path, GLenum shaderType)
{
    std::ifstream in(path);
    std::stringstream stream;
    stream << in.rdbuf();

    String shaderCode = stream.str();
    if(shaderCode.size() == 0) {
        LOG_ERROR("No shader code!");
        return 0;
    }
    const char* code = shaderCode.c_str();
    GLint codeSize = (int)shaderCode.size();

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &code, &codeSize);
    glCompileShader(shader);

    GLint isCompile;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompile);
    if(!isCompile)
    {
        GLint len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        char* log = new char[len];
        glGetShaderInfoLog(shader, len, NULL, log);
        LOG_ERROR("Failed to compile shader: %s", log);
        delete[] log;
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

bool Shader::LinkProgram(GLuint program)
{
    glLinkProgram(program);
    int isLinked;
    glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
    if(!isLinked) {
        GLint len;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
        char* log = new char[len];
        glGetProgramInfoLog(program, len, NULL, log);
        LOG_ERROR("Failed to link program: %s", log);
        delete[] log;
    }

    return isLinked;
}

Shader::Shader(GLuint program, GLuint shader1, GLuint shader2) :
    mProgram(program), mShader1(shader1), mShader2(shader2)
{
}

Shader::~Shader()
{
    if(mShader1) {
        glDetachShader(mProgram, mShader1);
        glDeleteShader(mShader1);
    }
    if(mShader2) {
        glDetachShader(mProgram, mShader2);
        glDeleteShader(mShader2);
    }

    glDeleteProgram(mProgram);
}
