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

const unsigned int SCALE = 8;

int width = SCR_WIDTH / SCALE;
int height = SCR_HEIGHT / SCALE;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void setup_framebuffer(GLuint framebuffer, GLuint deathbuffer, GLuint texture);

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
    
    // glew initialization
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    
    framebuffer_size_callback(window, SCR_WIDTH, SCR_HEIGHT);

    //// Outputting info about opengl version-------
    std::cout << glGetString(GL_VERSION) << "\n";
    std::cout << "Start Old Tellevision Effect Program" << std::endl;

    // Create two shader programs
    Shader shader("src\\shaders\\v_shader.shader", "src\\shaders\\f_shader.shader");
    Shader base_shader("src\\shaders\\base_v_shader.shader", "src\\shaders\\base_f_shader.shader");

    float vertices[] = 
    {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

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

    // Create additional framebuffer
    GLuint framebuffer, texture, depthBuffer;

    glGenFramebuffers(1, &framebuffer);
    glGenTextures(1, &texture);
    glGenRenderbuffers(1, &depthBuffer);
    setup_framebuffer(framebuffer, depthBuffer, texture);

    double deltaTime = 0.0;
    double lastFrame = 0.0;

    // Main cycle
    while (!glfwWindowShouldClose(window))
    {
        double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        //// Render base image in small frame buffer
        
        setup_framebuffer(framebuffer, depthBuffer, texture);

        glEnable(GL_DEPTH_TEST);
        glViewport(0, 0, width, height);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        base_shader.use();

        // Create Model matrix
        glm::mat4 model = glm::mat4(1.0f);
        float time = glfwGetTime();
        model = glm::rotate(model, time, glm::vec3(0.5f, 1.0f, 0.0f));

        // Create Projection and View matrix
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)::width /  (float)::height, 0.1f, 100.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));

        base_shader.setMat4("model", model);
        base_shader.setMat4("view", view);
        base_shader.setMat4("projection", projection);

        // Draw cube
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        glBindVertexArray(0);
        glDisable(GL_DEPTH_TEST);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);


        //// Render all window with old tv effect
        glViewport(0, 0, width * SCALE, height * SCALE);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
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
    ::width = width / SCALE;
    ::height = height / SCALE;
}

void setup_framebuffer(GLuint framebuffer, GLuint depthbuffer, GLuint texture)
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthbuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Framebuffer is not complete!" << std::endl;
    }
}