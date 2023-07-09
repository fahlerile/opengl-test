#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

#include "ShaderProgram.hpp"

ShaderProgram::ShaderProgram(std::string vertex_path, std::string fragment_path)
{
    // process both shaders
    unsigned int vertex_shader = this->process_shader(vertex_path, vertex);
    unsigned int fragment_shader = this->process_shader(fragment_path, fragment);

    // link two shaders to a shader program and save it to this->id
    this->id = glCreateProgram();
    glAttachShader(this->id, vertex_shader);
    glAttachShader(this->id, fragment_shader);

    int success;
    char link_log[512];

    glLinkProgram(this->id);
    glGetProgramiv(this->id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(this->id, 512, NULL, link_log);
        std::cout << "Failed to link shader program " << vertex_path << " " << fragment_path << std::endl;
        std::cout << link_log << std::endl;
    }

    // delete unused shaders (we are already linked them in a program)
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

// Process a single shader (read from file and compile). Returns an OpenGL shader ID.
unsigned int ShaderProgram::process_shader(std::string path, ShaderType type)
{
    std::stringstream ss;
    std::ifstream filestream;
    std::string line, shader_source;

    try
    {
        filestream.open(path);
        while (std::getline(filestream, line))
        {
            ss << line << "\n";
        }
        shader_source = ss.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "Failed to read " << path << std::endl;
        exit(1);
    }

    unsigned int shader;
    const char *shader_source_c = shader_source.c_str();

    switch (type)
    {
        case ShaderType::vertex:
            shader = glCreateShader(GL_VERTEX_SHADER);
            break;
        case ShaderType::fragment:
            shader = glCreateShader(GL_FRAGMENT_SHADER);
            break;
    }

    int success;
    char compile_log[512];

    glShaderSource(shader, 1, &shader_source_c, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, compile_log);
        std::cout << "Failed to compile shader " << path << std::endl;
        std::cout << compile_log << std::endl;
        exit(1);
    }

    return shader;
}

void ShaderProgram::use()
{
    glUseProgram(this->id);
}

void ShaderProgram::delete_program()
{
    glDeleteProgram(this->id);
}

unsigned int ShaderProgram::get_uniform_location(const char *name)
{
    return glGetUniformLocation(this->id, name);
}