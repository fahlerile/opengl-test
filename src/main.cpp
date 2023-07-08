#define GLEW_STATIC

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

const int window_width = 800;
const int window_height = 600;
const char *window_title = "Hello, triangle!";

void resize_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow *window);

int main()
{
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return 1;
    }

    GLFWwindow *window = glfwCreateWindow(window_width, window_height, window_title,
                                          NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 2;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resize_callback);

    if (!glewInit())
    {
        std::cout << "Failed to initialize GLEW (OpenGL)" << std::endl;
        glfwTerminate();
        return 3;
    }

    while (!glfwWindowShouldClose(window))
    {
        process_input(window);

        glClearColor(1.0f, 0.0f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void process_input(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void resize_callback(GLFWwindow* window, int width, int height)
{
    // resize opengl viewport as we resize the window
    glViewport(0, 0, width, height);
}