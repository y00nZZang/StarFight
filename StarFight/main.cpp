// Std. Includes
#include <string>
#include <iostream>
#include <cmath>
#include <vector>

// GLEW
//#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// GL includes
#include <shader.h>
#include <cube.h>
#include <plane.h>
#include <Model.h>
#include "StarShip.h"
#include "Background.h"


// Globals
unsigned int SCR_WIDTH = 2400;
unsigned int SCR_HEIGHT = 1500;
GLFWwindow* mainWindow = NULL;
static int maxY = 6;
static int maxX = maxY * 1.6; // maximum x,y movement of starship

glm::vec3 originStarShipPos(0, -3, 0);


// camera position
glm::vec3 cameraPos(0.0f, 0.0f, -10.0f);
// Projection initialization
glm::mat4  projection = glm::perspective(glm::radians(90.0f),
    (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 110.0f);
// View initialization
glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
// Model initialization
glm::mat4 model = glm::mat4(1.0);;


// init Obj
StarShip* starShip;
Cube* cube;
Model* ourModel;
Background* background;

//texture
unsigned int backgroundTexture;

//shader
Shader* cubeShader = NULL;
Shader* gshader = NULL;

// Function prototypes
GLFWwindow* glAllInit(const char* title);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double x, double y);
unsigned int loadTexture(const char* path, bool vflip);
void render();


int main()
{
    mainWindow = glAllInit("StarFight");

    // load texture
    backgroundTexture = loadTexture("res/textures/galaxy.jpg", false);
    
    // load shader
    
    gshader = new Shader("global.vs", "global.fs");
    gshader->use();
    gshader->setMat4("projection", projection);
    gshader->setMat4("view", view);
    model = glm::scale(model, glm::vec3(1.0f));
    gshader->setMat4("model", model);
    
    cubeShader = new Shader("cube.vs", "cube.fs");
    cubeShader->use();
    cubeShader->setMat4("projection", projection);
    cubeShader->setMat4("view", view);
    model = glm::scale(model, glm::vec3(3.0f));
    cubeShader->setMat4("model", model);
    

    // create obj
    starShip = new StarShip(projection, view, originStarShipPos, cameraPos, maxX, maxY);
    background = new Background(projection, view, glm::vec3(0.0f, 0.0f, 40.0f), glm::vec2(80.0f, 50.0f));

    cube = new Cube();
    ourModel = new Model((GLchar*)"res/models/planet/planet.obj");

    // Game loop
    while (!glfwWindowShouldClose(mainWindow))
    {
        glfwPollEvents();

        render();

        glfwSwapBuffers(mainWindow);
    }

    glfwTerminate();
    return 0;
}

void render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    background->draw(backgroundTexture);
    starShip->draw();
    
    gshader->use();
    ourModel->Draw(gshader);
    
    cubeShader->use();
    glBindTexture(GL_TEXTURE_2D, backgroundTexture);
    cube->draw(cubeShader);
}

// glAllInit();
GLFWwindow* glAllInit(const char* title)
{
    GLFWwindow* window;

    // glfw: initialize and configure
    if (!glfwInit()) {
        printf("GLFW initialisation failed!");
        glfwTerminate();
        exit(-1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // glfw window creation
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, title, NULL, NULL);
    if (window == NULL) {
        cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    // OpenGL states
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    // Allow modern extension features
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        cout << "GLEW initialisation failed!" << endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(-1);
    }

    return window;
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path, bool vflip)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    if (vflip) stbi_set_flip_vertically_on_load(true);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        printf("texture %s loaded\n", path);
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        else {
            format = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (key == GLFW_KEY_D) {
        starShip->movement(0);
    }
    if (key == GLFW_KEY_A) {
        starShip->movement(1);
    }
    if (key == GLFW_KEY_S) {
        starShip->movement(2);
    }
    if (key == GLFW_KEY_W) {
        starShip->movement(3);
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
}

void cursor_position_callback(GLFWwindow* window, double x, double y) {
}
