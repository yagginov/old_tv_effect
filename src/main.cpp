#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>

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
    Shader base_shader("src\\shaders\\base_v_shader.shader", "src\\shaders\\base_f_shader.shader");

    WindowData windowData;
    windowData.shader = &shader;
    glfwSetWindowUserPointer(window, &windowData);

    framebuffer_size_callback(window, SCR_WIDTH, SCR_HEIGHT);

    float vertices[] = 
    {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f
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


    float vertices_2[] = 
    {
        -1.0, -1.0, 0.0, 0.0,
         1.0, -1.0, 1.0, 0.0,
         1.0,  1.0, 1.0, 1.0,

         1.0,  1.0, 1.0, 1.0,
        -1.0,  1.0, 0.0, 1.0,
        -1.0, -1.0, 0.0, 0.0
    };

    unsigned int VBO_full;
    glGenBuffers(1, &VBO_full);

    unsigned int VAO_full;
    glGenVertexArrays(1, &VAO_full);

    glBindVertexArray(VAO_full);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_full);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_2), vertices_2, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    //// Disabled VBO, VAO---------------
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);



    double deltaTime = 0.0;
    double lastFrame = 0.0;

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)(SCR_WIDTH / 4) /  (float)(SCR_HEIGHT / 4), 0.1f, 100.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    

    while (!glfwWindowShouldClose(window))
    {
        double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        



//-------------------------------------------------------------------//

        int width = SCR_WIDTH / 4;
        int height = SCR_HEIGHT / 4;

        GLuint framebuffer, texture;

        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "Framebuffer is not complete!" << std::endl;
        }

        glViewport(0, 0, width, height);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        base_shader.use();
        glm::mat4 model = glm::mat4(1.0f);
        float time = glfwGetTime();
        model = glm::rotate(model, time, glm::vec3(0.5f, 1.0f, 0.0f));

        base_shader.setMat4("model", model);
        base_shader.setMat4("view", view);
        base_shader.setMat4("projection", projection);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        glBindVertexArray(0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
//--------------------------------------------------------------------------------//

        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

        //// Render here ----------------------------------------------------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        shader.use();
        shader.setInt("renderedTexture", 0);

        glBindVertexArray(VAO_full);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindVertexArray(0);

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
    glViewport(0, 0, width, height);
}