#pragma once

#include <string>

enum ShaderType {vertex, fragment};

class ShaderProgram
{
    private:
        unsigned int process_shader(std::string path, ShaderType type);
        unsigned int id;
    public:
        ShaderProgram(std::string vertex_path, std::string fragment_path);
        void use();
        void delete_program();
        unsigned int get_uniform_location(const char *name);
};
