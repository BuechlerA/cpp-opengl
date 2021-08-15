#include "shader.hpp"

Shader::Shader()
{

}

Shader::~Shader()
{
    glDeleteProgram(shaderID);
}

void Shader::Build(const std::string &vertexpath, const std::string &fragmentpath)
{
    ShaderProgramSources sources = ParseShader(vertexpath, fragmentpath);
    shaderID = CompileShader(sources.VertexSource.c_str(), sources.FragmentSouce.c_str());
}

ShaderProgramSources Shader::ParseShader(const std::string &vertexpath, const std::string &fragmentpath)
{
    std::ifstream vertstream(vertexpath);
    std::ifstream fragstream(fragmentpath);

    std::string vert_line;
    std::string frag_line;

    std::stringstream vstream;
    std::stringstream fstream;

    while (std::getline(vertstream, vert_line))
    {
        vstream << vert_line << '\n';
    }
    while (std::getline(fragstream, frag_line))
    {
        fstream << frag_line << '\n';
    }
    
    std::string vert_str = vstream.str();
    std::string frag_str = fstream.str();

    return {vert_str, frag_str};
}

GLuint Shader::CompileShader(const GLchar* vert_str, const GLchar* frag_str)
{
    GLuint shaderProgram = glCreateProgram();
    GLuint vertexShaderHandle = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);

    std::cout << "Compiling Shader..." << std::endl;
    std::cout << "shader ID:" << shaderProgram << std::endl;
    std::cout << "vertexshader ID:" << vertexShaderHandle << std::endl;
    std::cout << "fragmentshader ID:" << fragmentShaderHandle << std::endl;
    
    {
        glShaderSource(vertexShaderHandle, 1, &vert_str, nullptr);
        glCompileShader(vertexShaderHandle);
        int params = -1;
        glGetShaderiv(vertexShaderHandle, GL_COMPILE_STATUS, &params);
        if (GL_TRUE != params)
        {
            std::cerr << "ERROR: Vertex Shader index " << vertexShaderHandle << " did not compile" << std::endl;
            const int max_length = 2048;
            int actual_length = 0;
            char slog[2048];
            glGetShaderInfoLog(vertexShaderHandle, max_length, &actual_length, slog);
            std::cerr << "shader info log for GL index " << vertexShaderHandle << ": " << slog << std::endl;
            glDeleteShader(vertexShaderHandle);
            glDeleteShader(fragmentShaderHandle);
            glDeleteProgram(shaderProgram);
            return 0;
        }
    }
    {
        glShaderSource(fragmentShaderHandle, 1, &frag_str, nullptr);
        glCompileShader(fragmentShaderHandle);
        int params = -1;
        glGetShaderiv(fragmentShaderHandle, GL_COMPILE_STATUS, &params);
        if (GL_TRUE != params)
        {
            std::cerr << "ERROR: Fragment Shader index " << fragmentShaderHandle << " did not compile" << std::endl;
            const int max_length = 2048;
            int actual_length = 0;
            char slog[2048];
            glGetShaderInfoLog(fragmentShaderHandle, max_length, &actual_length, slog);
            std::cerr << "shader info log for GL index " << fragmentShaderHandle << ": " << slog << std::endl;
            glDeleteShader(vertexShaderHandle);
            glDeleteShader(fragmentShaderHandle);
            glDeleteProgram(shaderProgram);
            return 0;
        }
    }
    glAttachShader(shaderProgram, vertexShaderHandle);
    glAttachShader(shaderProgram, fragmentShaderHandle);
    {
        glLinkProgram(shaderProgram);
        glDeleteShader(vertexShaderHandle);
        glDeleteShader(fragmentShaderHandle);
        int params = -1;
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &params);
        if (GL_TRUE != params)
        {
            std::cerr << "ERROR: could not link shader program GL index: " << shaderProgram << std::endl;
            const int max_length = 2048;
            int actual_length = 0;
            char plog[2048];
            glGetProgramInfoLog(shaderProgram, max_length, &actual_length, plog);
            std::cerr << "proram info log for GL index " << shaderProgram << ": " << plog << std::endl;
            glDeleteProgram(shaderProgram);
            return 0;
        }
    }
    return shaderProgram;
}

void Shader::Bind() const
{
    glUseProgram(shaderID);
}

void Shader::Unbind() const
{
    glUseProgram(0);
    glDeleteProgram(shaderID);
}

void Shader::SetFragDataLocation(int colorNumber, const std::string &name)
{
    glBindFragDataLocation(shaderID, colorNumber, name.c_str());
}

void Shader::SetVertexAttribLocation(const std::string &name, int attribSize, GLenum attribType, GLboolean normalized, GLsizei stride, const GLvoid* pointer)
{
    GLint location = glGetAttribLocation(shaderID, name.c_str());
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, attribSize, attribType, normalized, stride, pointer);
}

void Shader::SetUniform1i(const std::string &name, int v0)
{
    glUniform1i(GetUniformLocation(name), v0);
}

void Shader::SetUniform1f(const std::string &name, float v0)
{
    glUniform1f(GetUniformLocation(name), v0);
}

void Shader::SetUniform2f(const std::string &name, float v0, float v1)
{
    glUniform2f(GetUniformLocation(name), v0, v1);
}

void Shader::SetUniform3f(const std::string &name, float v0, float v1, float v2)
{
    glUniform3f(GetUniformLocation(name), v0, v1, v2);
}

void Shader::SetUniform3fv(const std::string &name, int count, const float* value)
{
    glUniform3fv(GetUniformLocation(name), count, value);
}

void Shader::SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3)
{
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

int Shader::GetUniformLocation(const std::string &name)
{
    int location = glGetUniformLocation(shaderID, name.c_str());
    return location;
}