#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

const unsigned int SCR_WIDTH = 1500;
const unsigned int SCR_HEIGHT = 800; 

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

struct WindowData {
    Shader* shader;
};

int main()
{
    GLFWwindow* window;

    //// Initialize the library---
    if (!glfwInit()) 
    {
        return -1;
    }

    //// Create a windowed mode window and its OpenGL context-------
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Old Tellevision Style", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    //// Make the window's context current-------
    glfwMakeContextCurrent(window);

    //// init callback functions----------------------
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    //// Outputting info about opengl version-------
    std::cout << glGetString(GL_VERSION) << "\n";
    std::cout << "Start Old Tellevision Effect Program" << std::endl;

    Shader shader("src\\shaders\\v_shader.shader", "src\\shaders\\f_shader.shader");
    shader.use();
    shader.setVec2("resolution", glm::vec2(SCR_WIDTH, SCR_HEIGHT));

    WindowData windowData;
    windowData.shader = &shader;
    glfwSetWindowUserPointer(window, &windowData);

    framebuffer_size_callback(window, SCR_WIDTH, SCR_HEIGHT);

    glm::vec3 vertices[] = 
    {
        glm::vec3(-0.5, -0.5, 0.0),
        glm::vec3( 0.5, -0.5, 0.0),
        glm::vec3( 0.5,  0.5, 0.0),

        glm::vec3( 0.5,  0.5, 0.0),
        glm::vec3(-0.5,  0.5, 0.0),
        glm::vec3(-0.5, -0.5, 0.0)
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    //// Disabled VBO, VAO---------------
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    double deltaTime = 0.0;
    double lastFrame = 0.0;

    while (!glfwWindowShouldClose(window))
    {
        double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        //// Render here ----------------------------------------------------
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Рендеринг куба
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Swap front and back buffers---
        glfwSwapBuffers(window);

        // Poll for and process events---
        glfwPollEvents();
    }

    return 0;
}

void processInput(GLFWwindow* window)
{

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    WindowData* windowData = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
    windowData->shader->use();
    windowData->shader->setVec2("resolution", glm::vec2(width, height));

    glViewport(0, 0, width, height);
}