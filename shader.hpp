#pragma once
#include "commons.hpp"
#include <string>
#include <sstream>
#include <fstream>

struct ShaderProgramSources
{
    std::string VertexSource;
    std::string FragmentSouce;
};

class Shader
{
private:
    std::string vertexFilePath;
    std::string fragmentFilePath;
public:
    int shaderID;
    Shader();
    ~Shader();

    void Build(const std::string &vertexpath, const std::string &fragmentpath);
    void Bind() const;
    void Unbind() const;

    void SetFragDataLocation(int colorNumber, const std::string &name);
    void SetVertexAttribLocation(const std::string &name, int attribSize, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
    void SetUniform1i(const std::string &name, int v0);
    void SetUniform1f(const std::string &name, float v0);
    void SetUniform2f(const std::string &name, float v0, float v1);
    void SetUniform3f(const std::string &name, float v0, float v1, float v2);
    void SetUniform3fv(const std::string &name, int count, const float* value);
    void SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3);
private:
    ShaderProgramSources ParseShader(const std::string &vertexpath, const std::string &fragmentpath);
    GLuint CompileShader(const GLchar* vert_str, const GLchar* frag_str);
    int GetUniformLocation(const std::string &name);
};