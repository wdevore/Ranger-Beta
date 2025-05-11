#include <fstream>
#include <iostream>
#include <sstream>
// #include <limits.h> // For PATH_MAX (POSIX)
// #include <unistd.h> // For readlink (POSIX)

#include <shader.hpp>

namespace Core
{
    ErrorConditions Shader::read()
    {
        std::cout << "Shader::read reading shaders" << std::endl;

        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        // ensures ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            // char path[PATH_MAX];
            // ssize_t len = readlink("/proc/self/exe", path, sizeof(path) - 1);
            // if (len != -1)
            // {
            //     path[len] = '\0';
            //     std::string executablePath{path};
            //     std::cout << "Shader::read : " << executablePath << std::endl;
            // }

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
            return ErrorConditions::GLShaderFileNotFound;
        }

        return ErrorConditions::None;
    }

    ErrorConditions Shader::compile()
    {
        std::cout << "Shader::compile compiling shaders" << std::endl;

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
        ErrorConditions checkStatus = checkCompileErrors(vertex, "VERTEX");
        if (checkStatus != ErrorConditions::None)
            return checkStatus;

        // Fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkStatus = checkCompileErrors(fragment, "FRAGMENT");
        if (checkStatus != ErrorConditions::None)
            return checkStatus;

        // Shader Program
        program_ = glCreateProgram();

        glAttachShader(program_, vertex);
        glAttachShader(program_, fragment);

        glLinkProgram(program_);
        checkStatus = checkCompileErrors(program_, "PROGRAM");
        if (checkStatus != ErrorConditions::None)
            return checkStatus;

        // Delete the shaders as they're linked into our program now and are no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);

        return ErrorConditions::None;
    }

    ErrorConditions Shader::checkCompileErrors(GLuint shader, const std::string &type)
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
                return ErrorConditions::GLShaderCompileError;
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
                return ErrorConditions::GLShaderLinkError;
            }
        }

        return ErrorConditions::None;
    }

    ErrorConditions Shader::build()
    {
        std::cout << "Shader::build" << std::endl;

        ErrorConditions readStatus = read();
        if (readStatus != ErrorConditions::None)
            return readStatus;
        std::cout << "Shader::build read shaders" << std::endl;

        ErrorConditions compileStatus = compile();
        if (compileStatus != ErrorConditions::None)
            return compileStatus;
        std::cout << "Shader::build compiled shaders" << std::endl;

        return ErrorConditions::None;
    }

    void Shader::use()
    {
        glUseProgram(program_);
    }

} // namespace Core
