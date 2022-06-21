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

    glLinkProgram(program);

    return std::make_shared<Shader>(program);
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

    return shader;
}

Shader::Shader(GLuint program) :
    mProgram(program)
{
}

Shader::~Shader()
{
}
