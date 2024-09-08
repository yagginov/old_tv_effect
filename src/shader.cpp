#include "shader.h"

Shader::Shader(const std::string& name_vShader, const std::string& name_fShader) {
    this->ID = this->createShader(name_vShader, name_fShader);
}

//A function to compile shaders from a file. We pass the shader type as a parameter and we pass filename
// with name shader files
unsigned int Shader::compileShader(const std::string& path, unsigned int type) const
{
    std::string code;
    std::ifstream shaderFile;

    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        shaderFile.open(path);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        code = shaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* shaderCode = code.c_str();

    unsigned int shader;
    shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        int length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char* infoLog = (char*)_alloca(length * sizeof(char));
        glGetShaderInfoLog(shader, length, &length, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

// Function for creating a shader program
unsigned int Shader::createShader(const std::string& name_vShader, const std::string& name_fShader) const
{
    // Create shader program
    unsigned int program = glCreateProgram();
    // Create 2 shaders
    unsigned int vs = this->compileShader(name_vShader, GL_VERTEX_SHADER);
    unsigned int fs = this->compileShader(name_fShader, GL_FRAGMENT_SHADER);

    // Attaching each shader to a shader program
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    // Linking of a program
    glLinkProgram(program);
    // Checks to see whether the executables contained in program can execute given the current OpenGL state.
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

void Shader::use()
{
    glUseProgram(this->ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, glm::mat4 value) const
{
    glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVec3(const std::string& name, glm::vec3 value) const
{
    glUniform3fv(glGetUniformLocation(this->ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setVec2(const std::string& name, glm::vec2 value) const
{
    glUniform2fv(glGetUniformLocation(this->ID, name.c_str()), 1, glm::value_ptr(value));
}