#include <iostream>
#include <cmath>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "ShaderProgram.hpp"

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double x, double y);

const unsigned int window_width = 800;
const unsigned int window_height = 600;
const char *window_title = "Hello, box!";

glm::vec3 camera_pos(0.0f, 0.0f, 3.0f);
glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f,  0.0f);
const float camera_speed = 1.0f;

float yaw = -90.0f;
float pitch = 0.0f;

// mouse
int last_x = 0;
int last_y = 0;
bool first_mouse_input = true;
float sensitivity = 0.1f;

float delta_time = 0.0f;
float last_frame_time = 0.0f;

int main()
{
    // initialize GLFW
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return 1;
    }

    // create a window
    GLFWwindow* window = glfwCreateWindow(window_width, window_height,
                                          window_title, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 2;
    }

    // create window context (for OpenGL)
    glfwMakeContextCurrent(window);

    // set event callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // initialize GLEW
    if (glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return 3;
    }

    glEnable(GL_DEPTH_TEST);

    // initialize a shader program from basic shaders
    ShaderProgram shader("../res/shaders/basic/vertex.glsl",
                         "../res/shaders/basic/fragment.glsl");
    shader.use();


    // initialize a box texture
    stbi_set_flip_vertically_on_load(true);

    unsigned int box_texture;
    glGenTextures(1, &box_texture);
    glBindTexture(GL_TEXTURE_2D, box_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, num_channels;
    unsigned char *data = stbi_load("../res/textures/box.jpg", &width, &height, &num_channels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
        glfwTerminate();
        return 4;
    }
    stbi_image_free(data);

    // initialize stone texture
    unsigned int pekora_texture;
    glGenTextures(1, &pekora_texture);
    glBindTexture(GL_TEXTURE_2D, pekora_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("../res/textures/stone.jpg", &width, &height, &num_channels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
        glfwTerminate();
        return 4;
    }
    stbi_image_free(data);

    // vertices array (position, texture coords)
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,   1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f
    };

    glm::vec3 positions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    // creating VAO and VBO
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // loading data to a VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // setting attibutes to a VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // UNbinding VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // set texture uniforms
    glUniform1i(shader.get_uniform_location("Texture1"), 0);
    glUniform1i(shader.get_uniform_location("Texture2"), 1);

    // define coordinate transformation matrices
    glm::mat4 model(1.0f);
    glm::mat4 view;

    glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float) window_width / window_height, 0.1f, 100.0f);

    // set matrices uniforms
    glUniformMatrix4fv(shader.get_uniform_location("projection"), 1, GL_FALSE, glm::value_ptr(projection));

    while (!glfwWindowShouldClose(window))
    {
        float current_frame_time = glfwGetTime();
        delta_time = current_frame_time - last_frame_time;

        std::cout << "Frametime: " << delta_time << std::endl;

        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // update camera
        view = glm::mat4();
        view = glm::lookAt(camera_pos,
                           camera_pos + camera_front,
                           camera_up);
        glUniformMatrix4fv(shader.get_uniform_location("view"), 1, GL_FALSE, glm::value_ptr(view));

        shader.use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, box_texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, pekora_texture);

        glBindVertexArray(VAO);
        for (int i = 0; i < 10; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, positions[i]);
            model = glm::rotate(model, 1.0f * i, glm::vec3(1.0f, 1.0f, 0.0f));
            glUniformMatrix4fv(shader.get_uniform_location("model"), 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

        last_frame_time = current_frame_time;
    }

    // clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    shader.delete_program();

    glfwTerminate();
    return 0;
}

// Processes user input
void processInput(GLFWwindow *window)
{
    float camera_speed_normalized = delta_time * camera_speed;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera_pos += (camera_speed_normalized * camera_front);
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera_pos -= (camera_speed_normalized * camera_front);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera_pos -= (camera_speed_normalized * glm::normalize(glm::cross(camera_front, camera_up)));
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera_pos += (camera_speed_normalized * glm::normalize(glm::cross(camera_front, camera_up)));

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera_pos += (camera_speed_normalized * camera_up);
    else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera_pos -= (camera_speed_normalized * camera_up);
}

// Resize callback. Keeps viewport equal to actual size of a window.
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double x, double y)
{
    if (first_mouse_input)
    {
        first_mouse_input = false;
        last_x = x;
        last_y = y;
    }

    yaw += (x - last_x) * sensitivity;
    pitch -= (y - last_y) * sensitivity;  // minus here because I don't want inverted mouse controls

    last_x = x;
    last_y = y;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 camera_direction(cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
                               sin(glm::radians(pitch)),
                               sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
    camera_front = glm::normalize(camera_direction);
}
