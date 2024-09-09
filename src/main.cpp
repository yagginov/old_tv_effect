#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <cstdlib>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "stb_image.h"

#include "shader.h"
#include "camera.h"

const unsigned int SCR_WIDTH = 1500;
const unsigned int SCR_HEIGHT = 800; 


unsigned int scale = 4;

// Turning Speed
const float ANGLE_PER_SECONDS = 90.0f;
const float POINTS_PER_SECONDS = 0.5f;

// Small framebuffer size
int width = SCR_WIDTH / scale;
int height = SCR_HEIGHT / scale;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void processInput(GLFWwindow* window, float delta);
void setup_framebuffer(GLuint framebuffer, GLuint deathbuffer, GLuint texture);
void load_texture(GLuint texture, std::string path, float& aspect, GLuint texture_unit);

struct WindowData {
    Camera* camera;
    glm::mat4 model;
};


// The user needs to specify arguments when starting the program:
// filePath - path to the photo that will be used as a texture.
// scale - value (4, 8, 12, 16...) that indicates the size of the pixels in relation to the normal one.
int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <path_to_file> <scale_value>\n";
        return 1;
    }

    std::string filePath = argv[1];

    ::scale = std::atoi(argv[2]);
    
    if (::scale == 0)
    {
        std::cerr << "Error: scale must be a positive integer.\n";
        return 1;
    }
    ::width = SCR_WIDTH / scale;
    ::height = SCR_HEIGHT / scale;

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
    glfwSetScrollCallback(window, scroll_callback);
    
    // glew initialization
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    //// Outputting info about opengl version-------
    std::cout << glGetString(GL_VERSION) << "\n";
    std::cout << "Start Old Tellevision Effect Program" << std::endl;

    // Create camera
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

    // Create window data
    WindowData windowData;
    windowData.camera = &camera;
    windowData.model = *(new glm::mat4(1.0));

    glfwSetWindowUserPointer(window, &windowData);

    // Create two shader programs
    Shader shader("src\\shaders\\v_shader.shader", "src\\shaders\\f_shader.shader");
    Shader base_shader("src\\shaders\\base_v_shader.shader", "src\\shaders\\base_f_shader.shader");

    // Set scalling value to shader
    shader.use();
    shader.setVec2("scale", glm::vec2(::scale / 4, ::scale / 4));

    // Load your texture
    float aspect = 1.0;
    GLuint photo_texture;
    glGenTextures(1, &photo_texture);
    load_texture(photo_texture, filePath, aspect, GL_TEXTURE1);

    base_shader.use();
    base_shader.setInt("texture_01", 1);

    // Creating a textured sheet
    float vertices[] = 
    {
        -0.5f, -0.5f, -0.001f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.001f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.001f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.001f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.001f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.001f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.001f,  0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.001f,  0.0f,  0.0f,  1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.001f,  0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.001f,  0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.001f,  0.0f,  0.0f,  1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.001f,  0.0f,  0.0f,  1.0f,   0.0f, 0.0f
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    //// Disabled VBO, VAO---------------
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    // Create a rectangle that fills the entire screen
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

        processInput(window, deltaTime);

        //// Render base image in small frame buffer
        setup_framebuffer(framebuffer, depthBuffer, texture);

        glEnable(GL_DEPTH_TEST);
        glViewport(0, 0, width, height);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        base_shader.use();

        // Create Model matrix
        glm::mat4 model = glm::mat4(1.0f);
        model = windowData.model;
        model = glm::scale(model, glm::vec3(1.5f * aspect, 1.5f, 1.0f));

        // Create Projection and View matrix
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)::width /  (float)::height, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        base_shader.setMat4("model", model);
        base_shader.setMat4("view", view);
        base_shader.setMat4("projection", projection);

        // Draw cube
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);
        
        glBindVertexArray(0);
        glDisable(GL_DEPTH_TEST);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        //// Render full window with old tv effect
        glViewport(0, 0, width * scale, height * scale);

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


    // Memory cleat up
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO_full);
    glDeleteVertexArrays(1, &VAO_full);

    glDeleteTextures(1, &texture);
    glDeleteRenderbuffers(1, &depthBuffer);
    glDeleteFramebuffers(1, &framebuffer);

    glDeleteTextures(1, &photo_texture);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void processInput(GLFWwindow* window, float delta)
{
    WindowData* windowData = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

    // Closing the program
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) 
    {
        glfwSetWindowShouldClose(window, true);
    }

    // Setting image rotation
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
    {
        windowData->model = glm::rotate(windowData->model, glm::radians(ANGLE_PER_SECONDS * delta), glm::vec3(-1.0f, 0.0f, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        windowData->model = glm::rotate(windowData->model, glm::radians(ANGLE_PER_SECONDS * delta), glm::vec3(1.0f, 0.0f, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        windowData->model = glm::rotate(windowData->model, glm::radians(ANGLE_PER_SECONDS * delta), glm::vec3(0.0f, 1.0f, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        windowData->model = glm::rotate(windowData->model, glm::radians(ANGLE_PER_SECONDS * delta), glm::vec3(0.0f, -1.0f, 0.0f));
    }

    // Setting move
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) 
    {
        windowData->camera->add_position(glm::vec3(0.0f, POINTS_PER_SECONDS * delta, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        windowData->camera->add_position(glm::vec3(0.0f, -POINTS_PER_SECONDS * delta, 0.0f));    
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        windowData->camera->add_position(glm::vec3(-POINTS_PER_SECONDS * delta, 0.0f, 0.0f));    
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        windowData->camera->add_position(glm::vec3(POINTS_PER_SECONDS * delta, 0.0f, 0.0f));    
    }

    // Reset image rotation and camera movement
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) 
    {
        windowData->model = glm::rotate(glm::mat4(1.0), glm::radians(0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        windowData->camera->reset();
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    ::width = width / scale;
    ::height = height / scale;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    // Zoom
    WindowData* windowData = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
    windowData->camera->ProcessMouseScroll(yoffset);
}

void setup_framebuffer(GLuint framebuffer, GLuint depthbuffer, GLuint texture)
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glActiveTexture(GL_TEXTURE0);
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

void load_texture(GLuint texture, std::string path, float& aspect, GLuint texture_unit)
{
    glActiveTexture(texture_unit);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

    aspect = (float)width / height;

    if (data) 
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Error: Could not to open texture file: " << path << std::endl;
    }
    stbi_image_free(data);

    
}