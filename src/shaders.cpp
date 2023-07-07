#define GLEW_STATIC
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shaders.h"

namespace shaders_sources
{
	const char* vertex = "#version 330 core\n"
						 "layout (location = 0) in vec3 aPos;\n"
						 "void main()\n"
						 "{\n"
						 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
						 "}\0";

	const char* fragment = "#version 330 core\n"
						   "out vec4 FragColor;\n"
						   "void main()\n"
						   "{\n"
						   "	FragColor = vec4(0.7f, 0.0f, 0.2f, 1.0f);"
						   "}\n";

}

// Create shaders. NEED TO INIT GLEW BEFORE RUNNING THIS!
int create_shaders()
{
	int shader_compile_success;
	char compile_log[512];

	// compile the vertex shader
	unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &shaders_sources::vertex, NULL);
	glCompileShader(vertex_shader);

	// check if there are compile errors for vertex shader
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &shader_compile_success);
	if (!shader_compile_success)
	{
		glGetShaderInfoLog(vertex_shader, 512, NULL, compile_log);
		std::cout << "shaders_sources::vertex\n" << compile_log << std::endl;
		return -1;
	}

	// compile the fragment shader
	unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &shaders_sources::fragment, NULL);
	glCompileShader(fragment_shader);

	// check if there are compile errors for fragment shader
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &shader_compile_success);
	if (!shader_compile_success)
	{
		glGetShaderInfoLog(fragment_shader, 512, NULL, compile_log);
		std::cout << "shader_compile_success\n" << compile_log << std::endl;
		return -2;
	}

	// create shader program
	unsigned int shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	// check if there are compile errors for shader program
	glGetProgramiv(shader_program, GL_LINK_STATUS, &shader_compile_success);
	if (!shader_compile_success) {
		glGetProgramInfoLog(shader_program, 512, NULL, compile_log);
		std::cout << "shader_program\n" << compile_log << std::endl;
		return -3;
	}

	// glUseProgram(shader_program);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return shader_program;
}