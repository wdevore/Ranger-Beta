#include <fstream>
#include <iostream>
#include <sstream>

#include <shader.hpp>

namespace Core
{
    void Shader::read()
    {
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        // ensures ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            // Open files
            vShaderFile.open(vertexPath_);
            fShaderFile.open(fragmentPath_);

            std::stringstream vShaderStream, fShaderStream;
            // Read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            // close file handlers
            vShaderFile.close();
            fShaderFile.close();

            // Convert stream into string
            vertexCode_ = vShaderStream.str();
            fragmentCode_ = fShaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            std::cerr << "ERROR::SHADER::FAILED_TO_READ_FILE: " << e.what() << std::endl;
        }
    }

    void Shader::compile()
    {
        const GLchar *vShaderCode = vertexCode_.c_str();
        const GLchar *fShaderCode = fragmentCode_.c_str();

        // 2. Compile shaders
        GLuint vertex, fragment;
        GLint success;
        GLchar infoLog[512];

        // Vertex Shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");

        // Fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");

        // Shader Program
        program_ = glCreateProgram();

        glAttachShader(program_, vertex);
        glAttachShader(program_, fragment);

        glLinkProgram(program_);
        checkCompileErrors(program_, "PROGRAM");

        // Delete the shaders as they're linked into our program now and are no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void Shader::checkCompileErrors(GLuint shader, const std::string &type)
    {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cerr << "| ERROR::::SHADER-COMPILATION-ERROR of type: " << type << "|\n"
                          << infoLog << "\n| -- --------------------------------------------------- -- |" << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cerr << "| ERROR::::PROGRAM-LINKING-ERROR of type: " << type << "|\n"
                          << infoLog << "\n| -- --------------------------------------------------- -- |" << std::endl;
            }
        }
    }

    void Shader::build()
    {
        read();
        compile();
    }

    void Shader::use()
    {
        glUseProgram(program_);
    }

} // namespace Core
