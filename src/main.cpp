#define GLEW_STATIC
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shaders.h"

const int width = 800;
const int height = 600;
const char* title = "Hello, Triangle!";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

int main()
{
	// GLFW initialization
	if (!glfwInit())
		return -1;

	// create window
	GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!window)  // if failed to create a window
	{
		glfwTerminate();
		return -2;
	}

	glfwSetWindowPos(window, 100, 100);  // set the position of opened window
	glfwMakeContextCurrent(window);  // create OpenGL context for window
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// OpenGL initialization (glew)
	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		return -3;
	}

	// Print current OpenGL version
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	// create shaders (exit if failure)
	int shader_program = create_shaders();
	if (shader_program < 0)
	{
		glfwTerminate();
		return -4;
	}

	// array with vertex positions
	float first_triangle[] = {
		-0.9f, -0.5f, 0.0f,
		-0.0f, -0.5f, 0.0f,
		-0.45f, 0.5f, 0.0f,
	};
	float second_triangle[] = {
		0.0f, -0.5f, 0.0f,
		0.9f, -0.5f, 0.0f,
		0.45f, 0.5f, 0.0f
	};

	unsigned int VBOs[2], VAOs[2];

	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);

	// First triangle
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(first_triangle), first_triangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	// Second triangle
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(second_triangle), second_triangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	while (!glfwWindowShouldClose(window))
	{
		process_input(window);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader_program);

		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	glDeleteProgram(shader_program);

	glfwTerminate();  // close glfw window
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}