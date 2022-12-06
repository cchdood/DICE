#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stb/stb_image.h>


#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

#include "Keyboard.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void setTexture(unsigned int &texture, const char* path);

glm::mat4 mouseTransform = glm::mat4(1.0f);

unsigned int screen_width = 800, screen_height = 600;
float x, y, z, dx, dy, dz, rad, cam_rad;

int main(void)
{

    GLFWwindow* window;

    // Initialize the library 
    glfwInit();

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(screen_width, screen_height, "Hello World", NULL, NULL);
    if (!window)
    {
        std::cout << "error occurs in creating GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "error occurs in initializing glad" << std::endl;
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, screen_width, screen_height);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetKeyCallback(window, Keyboard::keyCallback);

    glEnable(GL_DEPTH_TEST);

    // complie vertex shader and fragment shader
    Shader shader("assets/vertex_core.glsl", "assets/fragment_core.glsl");

    //data matrix
    float v1[] = {
        // position                texture         face index

        // cube face 1
        -0.5f, -0.5f, -0.5f,       0.0f, 0.0f,     1.0f,
         0.5f, -0.5f, -0.5f,       1.0f, 0.0f,     1.0f,
         0.5f,  0.5f, -0.5f,       1.0f, 1.0f,     1.0f,
        -0.5f,  0.5f, -0.5f,       0.0f, 1.0f,     1.0f,

        // cube face 2
        -0.5f, -0.5f,  0.5f,       0.0f, 0.0f,     2.0f,
         0.5f, -0.5f,  0.5f,       1.0f, 0.0f,     2.0f,
         0.5f,  0.5f,  0.5f,       1.0f, 1.0f,     2.0f,
        -0.5f,  0.5f,  0.5f,       0.0f, 1.0f,     2.0f,

        // cube face 3
        -0.5f, -0.5f, -0.5f,       0.0f, 0.0f,     3.0f,
         0.5f, -0.5f, -0.5f,       1.0f, 0.0f,     3.0f,
         0.5f, -0.5f,  0.5f,       1.0f, 1.0f,     3.0f,
        -0.5f, -0.5f,  0.5f,       0.0f, 1.0f,     3.0f,

        // cube face 4
        -0.5f,  0.5f, -0.5f,       0.0f, 0.0f,     4.0f,
         0.5f,  0.5f, -0.5f,       1.0f, 0.0f,     4.0f,
         0.5f,  0.5f,  0.5f,       1.0f, 1.0f,     4.0f,
        -0.5f,  0.5f,  0.5f,       0.0f, 1.0f,     4.0f,

        // cube face 5
        -0.5f, -0.5f, -0.5f,       0.0f, 0.0f,     5.0f,
        -0.5f,  0.5f, -0.5f,       1.0f, 0.0f,     5.0f,
        -0.5f,  0.5f,  0.5f,       1.0f, 1.0f,     5.0f,
        -0.5f, -0.5f,  0.5f,       0.0f, 1.0f,     5.0f,

        // cube face 6
         0.5f, -0.5f, -0.5f,       0.0f, 0.0f,     6.0f,
         0.5f,  0.5f, -0.5f,       1.0f, 0.0f,     6.0f,
         0.5f,  0.5f,  0.5f,       1.0f, 1.0f,     6.0f,
         0.5f, -0.5f,  0.5f,       0.0f, 1.0f,     6.0f,

        // tetra
         1.0f,  0.0f,  0.0f,       0.3f, 0.5f,     0.0f,
         0.0f,  1.0f,  0.0f,       0.2f, 0.7f,     0.0f,
         0.0f,  0.0f,  1.0f,       0.1f, 0.3f,     0.0f,
         0.0f,  0.0f,  0.0f,       0.4f, 0.8f,     0.0f
        
    }; 

    unsigned int indices[] = {
    0,  1,  3,   // cube face 1
    1,  2,  3,   

    4,  5,  7,   // cube face 2
    5,  6,  7,

    8,  9,  11,  // cube face 3
    9,  10, 11,

    12, 13, 15,  // cube face 4
    13, 14, 15,

    16, 17, 19,  // cube face 5
    17, 18, 19,

    20, 21, 23,  // cube face 6
    21, 22, 23,

    24, 25, 26,  // tetra face 1

    24, 25, 27,  // tetra face 2

    24, 26, 27,  // tetra face 3

    25, 26, 27   // tetra face 4

    };

    //create Vertex Buffer Obj and Vertex Array Obj
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //bind VAO
    glBindVertexArray(VAO);

    //bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(v1), v1, GL_STATIC_DRAW);

    //set EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //attibute ptr
    //positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //texture coordinate
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // texture
    unsigned int texture1, texture2, texture3, texture4, texture5, texture6;

    setTexture(texture1, "image/1.png");
    setTexture(texture2, "image/2.png");
    setTexture(texture3, "image/3.png");
    setTexture(texture4, "image/4.png");
    setTexture(texture5, "image/5.png");
    setTexture(texture6, "image/6.png");

    shader.activate();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);
    shader.setInt("texture3", 2);
    shader.setInt("texture4", 3);
    shader.setInt("texture5", 4);
    shader.setInt("texture6", 5);

    // initialize parameters for transition matrix

    x = 0.0f;
    y = -0.3f;
    z = 7.0f;
    dx = 1.0f;
    dy = 1.0f;
    dz = 1.0f;
    rad = 60.0f;
    cam_rad = 15.0f;

    while (!glfwWindowShouldClose(window))
    {
        //input
        processInput(window);

        //rendering
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);    // empty buffer color
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture3);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, texture4);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, texture5);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, texture6);

        //draw
        glBindVertexArray(VAO);

        glm::mat4 model = glm::mat4(1.0f);       // initialized as identity matrix 
        glm::mat4 model_tetra = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        model_tetra = glm::translate(model_tetra, glm::vec3(-1.0f, -1.0f, -1.0f));                   // model matrix for the tetra
        model_tetra = glm::rotate(model_tetra, glm::radians(60.0f), glm::vec3(1.0f, 2.0f, 1.0f));

        model = glm::translate(model, glm::vec3(dx, dy, dz));                                        // model matrix for the dice
        model = glm::rotate(model, glm::radians(rad), glm::vec3(0.0f, 1.0f, 1.0f));

        view = glm::translate(view, glm::vec3(-x, -y, -z));                                          // view matrix
        view = glm::rotate(view, glm::radians(cam_rad), glm::vec3(1.0f, 0.0f, 0.0f));
        
        projection = glm::perspective(glm::radians(45.0f), (float)screen_width / (float)screen_height, 0.1f, 100.0f);     // projection matrix
        // projection = glm::ortho(glm::radians(45.0f), (float)screen_width / (float)screen_height, 0.1f, 100.0f);

        shader.activate();

        shader.setMat4("model", model);
        shader.setMat4("model_tetra", model_tetra);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        // new frame to window
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    screen_width = width;
    screen_height = height;
}


void processInput(GLFWwindow* window)
{
    if (Keyboard::key(GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, true);
    }

    if (Keyboard::key(GLFW_KEY_UP)) {
        dz -= 0.1f;
    }

    if (Keyboard::key(GLFW_KEY_DOWN)) {
        dz += 0.1f;
    }

    if (Keyboard::key(GLFW_KEY_LEFT)) {
        dx -= 0.1f;
    }

    if (Keyboard::key(GLFW_KEY_RIGHT)) {
        dx += 0.1f;
    }

    if (Keyboard::key(GLFW_KEY_SPACE)) {
        dy += 0.1f;
    }

    if (Keyboard::key(GLFW_KEY_LEFT_CONTROL)) {
        dy -= 0.1f;
    }

    if (Keyboard::key(GLFW_KEY_E)) {
        rad -= 1.0f;
    }

    if (Keyboard::key(GLFW_KEY_Q)) {
        rad += 1.0f;
    }
}

void setTexture(unsigned int &texture, const char* path) {

    glGenTextures(1, &texture);                                         // gen texture
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);       // wrap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   // filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    int width, height, nChannels;                                       // load image
    unsigned char* data = stbi_load(path, &width, &height, &nChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else std::cout << "error occurs while loading the texture" << std::endl;
    stbi_image_free(data);
}
