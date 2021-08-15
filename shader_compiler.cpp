#include "shader_compiler.hpp"

GLuint compileShaderFromStrings(const GLchar* vert_str, const GLchar* frag_str);

GLuint parseShaderFromFiles(const std::string &vertexpath, const std::string &fragmentpath)
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

    return compileShaderFromStrings(vert_str.c_str(), frag_str.c_str());
}

GLuint compileShaderFromStrings(const GLchar* vert_str, const GLchar* frag_str)
{
    GLuint shaderProgram = glCreateProgram();
    GLuint vertexShaderHandle = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);

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
            std::cerr << "ERROR: Vertex Shader index " << fragmentShaderHandle << " did not compile" << std::endl;
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