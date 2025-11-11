// Std. Includes
#include <string>
#include <iostream>
#include <vector> 
#include <cmath>

#define SDL_MAIN_HANDLED  

// Librerias para audio
#include <SDL.h>
#include <SDL_mixer.h>


// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"
#include "Texture.h"
using namespace std;

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();


// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;

GLfloat lastFrame = 0.0f;
glm::vec3 convertBlenderToOpenGL(float x, float y, float z)
{
    // Blender usa Z hacia arriba - OpenGL usa Y hacia arriba
    return glm::vec3(x, z, -y);
}

glm::vec3 pointLightPositions[] = {
    convertBlenderToOpenGL(-20.474f, 47.899f, 5.0656f),   // Point.003
    convertBlenderToOpenGL(-6.0347f, 47.899f, 5.0656f),   // Point.005
    convertBlenderToOpenGL(-37.545f, 34.733f, 7.3384f),   // Point.001
    convertBlenderToOpenGL(-19.421f, 34.733f, 7.3384f),   // Point
    convertBlenderToOpenGL(-37.545f, 11.594f, 7.3384f),   // Point.002
    convertBlenderToOpenGL(-17.645f, 3.1487f, 5.9039f),   // Light
    convertBlenderToOpenGL(-31.336f, 47.899f, 5.0656f),   // Point 004
    convertBlenderToOpenGL(-20.437f, 61.292f, 5.0656f)    // Rojo
};

float vertices[] = {
     -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

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


//Variables globales para boton de musica
Mix_Music* music = nullptr;
bool musicOn = false;


int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Equipo 9 Proyecto", nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup y compilacion de shaders
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    Shader skinnedShader("Shader/_skin_runtime.vs", "Shader/_tex_runtime.frag");
    Shader skyboxShader("Shader/SkyBox.vs", "Shader/SkyBox.frag");



    // --- AUDIO SETUP ---
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cout << "Error inicializando SDL: " << SDL_GetError() << std::endl;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "Error inicializando SDL_mixer: " << Mix_GetError() << std::endl;
    }

    // Cargar musica (pero no se reproduce aun)
    music = Mix_LoadMUS("Audio/in the pool.mp3");
    if (!music) {
        std::cout << "No se pudo cargar la musica: " << Mix_GetError() << std::endl;
    }

    // Load models
    stbi_set_flip_vertically_on_load(false);
    Model backroom((char*)"Models/BackroomModel/backrooms.obj");
    Model balon((char*)"Models/balon_futbol.obj");
    Model animacion((char*)"Models/Baile2.fbx");
    Model animacion2((char*)"Models/baile1.fbx");
    Model animacion3((char*)"Models/jugador1.fbx");


    GLfloat skyboxVertices[] = {
        // Positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };

    GLuint indices[] =
    {  // Note that we start from 0!
        0,1,2,3,
        4,5,6,7,
        8,9,10,11,
        12,13,14,15,
        16,17,18,19,
        20,21,22,23,
        24,25,26,27,
        28,29,30,31,
        32,33,34,35
    };

    // First, set the container's VAO (and VBO)
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);


    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Set texture units
    lightingShader.Use();
    glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
    glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

    // SkyBox
    GLuint skyboxVBO, skyboxVAO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

    // Load textures
    vector < const GLchar*> faces;
    faces.push_back("SkyBox/left.jpg");
    faces.push_back("SkyBox/right.jpg");
    faces.push_back("SkyBox/top.jpg");
    faces.push_back("SkyBox/bottom.jpg");
    faces.push_back("SkyBox/front.jpg");
    faces.push_back("SkyBox/back.jpg");

    GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

	// Projection matrix
    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

   

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();

        // Clear the colorbuffer
        glClearColor(0.02f, 0.02f, 0.02f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightingShader.Use();
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);


        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Luz direccional 
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.01f, 0.01f, 0.01f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.03f, 0.03f, 0.03f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.02f, 0.02f, 0.02f);

        glm::vec3 white = glm::vec3(1.0f);
        glm::vec3 red = glm::vec3(1.0f, 0.0f, 0.0f); //  rojo 

        for (int i = 0; i < 8; i++) {
            std::string num = std::to_string(i);

            glm::vec3 color = white;
            if (i == 7) color = red;

            glUniform3fv(glGetUniformLocation(lightingShader.Program, ("pointLights[" + num + "].position").c_str()),
                1, glm::value_ptr(pointLightPositions[i]));

            // Luz ambiental tenue
            glUniform3fv(glGetUniformLocation(lightingShader.Program, ("pointLights[" + num + "].ambient").c_str()),
                1, glm::value_ptr(color * 0.1f));

            // Ajuste de intensidad para luces 003, 004 y 005 (indices 0, 1, 6)
            float diffuseIntensity = 2.0f;
            if (i == 0 || i == 1 || i == 6)
                diffuseIntensity = 0.6f; // mas suaves

            // Luz difusa
            glUniform3fv(glGetUniformLocation(lightingShader.Program, ("pointLights[" + num + "].diffuse").c_str()),
                1, glm::value_ptr(color * diffuseIntensity));

            // Reflejo especular tambien reducido en esas luces
            float specularIntensity = (i == 0 || i == 1 || i == 6) ? 0.5f : 1.0f;
            glUniform3fv(glGetUniformLocation(lightingShader.Program, ("pointLights[" + num + "].specular").c_str()),
                1, glm::value_ptr(color * specularIntensity));

            // Atenuacion (alcance de luz)
            glUniform1f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + num + "].constant").c_str()), 1.0f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + num + "].linear").c_str()), 0.07f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + num + "].quadratic").c_str()), 0.017f);
        }



        // Posicion de la camara
        glUniform3f(glGetUniformLocation(lightingShader.Program, "viewPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


        // Draw the loaded model
        glm::mat4 model(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        backroom.Draw(lightingShader);

        glm::mat4 model2(1);
        model2 = glm::translate(model2, glm::vec3(-20.0f, -2.0f, -30.0f)); // mismo Z que el otro
        model2 = glm::scale(model2, glm::vec3(1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model2));
        balon.Draw(lightingShader);


        double t = glfwGetTime();

        // PERSONA 1
        animacion.UpdateAnimation(t);
        std::vector<glm::mat4> bones1;
        animacion.GetBoneMatrices(bones1, 100);

        skinnedShader.Use();
        GLint bonesLoc = glGetUniformLocation(skinnedShader.Program, "bones");
        if (bonesLoc >= 0 && !bones1.empty())
            glUniformMatrix4fv(bonesLoc, (GLsizei)bones1.size(), GL_FALSE, &bones1[0][0][0]);

        glUniformMatrix4fv(glGetUniformLocation(skinnedShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
        glUniformMatrix4fv(glGetUniformLocation(skinnedShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // PERSONA 1
        glm::mat4 modelAnim1(1.0f);
        modelAnim1 = glm::translate(modelAnim1, glm::vec3(-20.0f, -2.0f, -60.0f)); // -8 en Z la empuja atras
        modelAnim1 = glm::scale(modelAnim1, glm::vec3(0.035f));
        glUniformMatrix4fv(glGetUniformLocation(skinnedShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelAnim1));

        animacion.Draw(skinnedShader);


        // PERSONA 2
        animacion2.UpdateAnimation(t);
        std::vector<glm::mat4> bones2;
        animacion2.GetBoneMatrices(bones2, 100);

        if (bonesLoc >= 0 && !bones2.empty())
            glUniformMatrix4fv(bonesLoc, (GLsizei)bones2.size(), GL_FALSE, &bones2[0][0][0]);

        // PERSONA 2
        glm::mat4 modelAnim2(1.0f);
        modelAnim2 = glm::translate(modelAnim2, glm::vec3(-16.0f, -2.0f, -60.0f)); // mismo Z que el otro
        modelAnim2 = glm::rotate(modelAnim2, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelAnim2 = glm::scale(modelAnim2, glm::vec3(0.035f));
        glUniformMatrix4fv(glGetUniformLocation(skinnedShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelAnim2));

        animacion2.Draw(skinnedShader);

        // JUGADOR
        animacion3.UpdateAnimation(t);
        std::vector<glm::mat4> bones3;
        animacion3.GetBoneMatrices(bones3, 100);

        if (bonesLoc >= 0 && !bones3.empty())
            glUniformMatrix4fv(bonesLoc, (GLsizei)bones3.size(), GL_FALSE, &bones3[0][0][0]);

        //JUGADOR
        glm::mat4 modelAnim3(1.0f);
        modelAnim3 = glm::translate(modelAnim3, glm::vec3(-20.0f, -2.0f, -30.0f)); // mismo Z que el otro
        modelAnim3 = glm::scale(modelAnim3, glm::vec3(0.035f));
        glUniformMatrix4fv(glGetUniformLocation(skinnedShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelAnim3));

        animacion3.Draw(skinnedShader);

        // Draw SkyBox
        glDepthFunc(GL_LEQUAL);
        skyboxShader.Use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);

        // Swap the buffers
        glfwSwapBuffers(window);

    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVAO);

    if (music) {
        Mix_FreeMusic(music);
        music = nullptr;
    }
    Mix_CloseAudio();
    SDL_Quit();

    glfwTerminate();
    return 0;

}


// Moves/alters the camera positions based on user input
void DoMovement()
{
    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }


}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }

    // Inicia musica con tecla M
    if (key == GLFW_KEY_M && action == GLFW_PRESS) {
        if (!musicOn) {
            if (music) {
                Mix_PlayMusic(music, -1); // loop
                musicOn = true;
                std::cout << "Musica ON\n";
            }
        }
        else {
            Mix_HaltMusic();
            musicOn = false;
            std::cout << "Musica OFF\n";
        }
    }



}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}