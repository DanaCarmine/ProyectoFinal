// Std. Includes
#include <string>
#include <iostream>

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

#include <vector>

// Estructura para keyframes
struct Keyframe {
    float time;
    glm::vec3 position;
    glm::vec3 rotation;
};



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

//Variables globales para boton de musica
Mix_Music* music = nullptr;
bool musicOn = false;

// Sistema de keyframes para la pelota
std::vector<Keyframe> keyframes;
int currentKeyframe = 0;
float animationTime = 0.0f;
bool playingKeyframes = false;
float animationDuration = 0.0f;
glm::vec3 ballPosition = glm::vec3(-20.3f, -1.9f, -28.0f);  // Posición inicial 
glm::vec3 ballRotation = glm::vec3(0.0f, 0.0f, 0.0f);
float playerAnimationTime = 0.0f;  
bool ballAnimationTriggered = false;
bool playerAnimationPlayed = false;
bool hasPlayedOnce = false;
bool animationActivated = false;

bool person1AnimationActive = false;
bool person2AnimationActive = false;
float person1AnimationStartTime = 0.0f;
float person2AnimationStartTime = 0.0f;
bool person1AnimationPlayed = false;
bool person2AnimationPlayed = false;
float person1FrozenTime = 0.0f;
float person2FrozenTime = 0.0f;

// Interpolación lineal
float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

glm::vec3 lerpVec3(glm::vec3 a, glm::vec3 b, float t) {
    return glm::vec3(
        lerp(a.x, b.x, t),
        lerp(a.y, b.y, t),
        lerp(a.z, b.z, t)
    );
}

// Configurar keyframes del tiro parabólico
void setupParabolicKeyframes() {
    keyframes.clear();

    // Keyframe 0: Posición inicial
    Keyframe kf0;
    kf0.time = 0.0f;
    kf0.position = glm::vec3(-20.3f, -1.9f, -28.0f);
    kf0.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    keyframes.push_back(kf0);

    // Keyframe 1: Momento del contacto (frame 13 a 30fps = 0.433 segundos)
    Keyframe kf1;
    kf1.time = 0.433f;  
    kf1.position = glm::vec3(-20.3f, -1.9f, -28.0f);
    kf1.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    keyframes.push_back(kf1);

    // Keyframe 2: Despegue (justo después del contacto)
    Keyframe kf2;
    kf2.time = 0.5f;  
    kf2.position = glm::vec3(-20.3f, -0.5f, -24.0f);
    kf2.rotation = glm::vec3(180.0f, 0.0f, 0.0f);
    keyframes.push_back(kf2);

    // Keyframe 3: Punto más alto
    Keyframe kf3;
    kf3.time = 1.2f;
    kf3.position = glm::vec3(-20.3f, 4.0f, -15.0f);
    kf3.rotation = glm::vec3(360.0f, 0.0f, 0.0f);
    keyframes.push_back(kf3);

    // Keyframe 4: Bajada (MÁS CERCA DEL SUELO)
    Keyframe kf4;
    kf4.time = 1.8f;
    kf4.position = glm::vec3(-20.3f, -0.5f, -6.0f);
    kf4.rotation = glm::vec3(540.0f, 0.0f, 0.0f);
    keyframes.push_back(kf4);

    // Keyframe 5: Aterrizaje (TOCANDO EL SUELO)
    Keyframe kf5;
    kf5.time = 2.2f;  
    kf5.position = glm::vec3(-20.3f, -1.9f, -2.0f);
    kf5.rotation = glm::vec3(720.0f, 0.0f, 0.0f);
    keyframes.push_back(kf5);

    // Keyframe 6: PAUSA en el suelo 
    Keyframe kf6;
    kf6.time = 3.5f;  
    kf6.position = glm::vec3(-20.3f, -1.9f, -2.0f);
    kf6.rotation = glm::vec3(720.0f, 0.0f, 0.0f);
    keyframes.push_back(kf6);

    animationDuration = keyframes.back().time;
}
// Interpolar entre keyframes
void interpolateKeyframes() {
    if (!playingKeyframes || keyframes.size() < 2) return;

    int nextKeyframe = currentKeyframe + 1;

    if (nextKeyframe >= keyframes.size()) {
        ballPosition = keyframes.back().position;
        ballRotation = keyframes.back().rotation;
        return; 
    }

    Keyframe& kf1 = keyframes[currentKeyframe];
    Keyframe& kf2 = keyframes[nextKeyframe];

    float t = (animationTime - kf1.time) / (kf2.time - kf1.time);
    t = glm::clamp(t, 0.0f, 1.0f);

    ballPosition = lerpVec3(kf1.position, kf2.position, t);
    ballRotation = lerpVec3(kf1.rotation, kf2.rotation, t);

    if (animationTime >= kf2.time) {
        currentKeyframe++;
    }
}

// Actualizar animación
void UpdateBallAnimation(float currentTime) {
    if (playingKeyframes) {
        animationTime = currentTime - playerAnimationTime;
        interpolateKeyframes();

        if (animationTime >= animationDuration) {
            playingKeyframes = false;
            ballPosition = keyframes.back().position;  
            ballRotation = keyframes.back().rotation;
            animationTime = 0.0f;
            currentKeyframe = 0;
            std::cout << "Pelota en suelo: Y = " << ballPosition.y << "\n";  
        }
    }
}
void CheckAndTriggerBallAnimation(float currentTime) {
    // Solo funciona si la animación fue activada con B
    if (!animationActivated || playingKeyframes) return;

    float timeInAnimation = currentTime - playerAnimationTime;
    float currentFrame = timeInAnimation * 30.0f;  // Convertir a frames (30 FPS)

    // Activar pelota en el frame 13
    if (currentFrame >= 12.0f && currentFrame <= 14.0f && !ballAnimationTriggered) {
        setupParabolicKeyframes();
        playingKeyframes = true;
        ballAnimationTriggered = true;
        std::cout << "✓ Pelota disparada en frame: " << currentFrame << "\n";
    }

    // Resetear trigger
    if (currentFrame < 12.0f || currentFrame > 14.0f) {
        ballAnimationTriggered = false;
    }
}



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
    Shader shader("Shader/lighting.vs", "Shader/lighting.frag");
    Shader skinnedShader("Shader/_skin_runtime.vs", "Shader/_tex_runtime.frag");



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

        double t = glfwGetTime();
        CheckAndTriggerBallAnimation(t);
        UpdateBallAnimation(t);  


        // Clear the colorbuffer
        glClearColor(0.02f, 0.02f, 0.02f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();
        glUniform1f(glGetUniformLocation(shader.Program, "material.shininess"), 16.0f);


        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Luz direccional 
        glUniform3f(glGetUniformLocation(shader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(shader.Program, "dirLight.ambient"), 0.01f, 0.01f, 0.01f);
        glUniform3f(glGetUniformLocation(shader.Program, "dirLight.diffuse"), 0.03f, 0.03f, 0.03f);
        glUniform3f(glGetUniformLocation(shader.Program, "dirLight.specular"), 0.02f, 0.02f, 0.02f);

        glm::vec3 white = glm::vec3(1.0f);
        glm::vec3 red = glm::vec3(1.0f, 0.0f, 0.0f); //  rojo 

        for (int i = 0; i < 8; i++) {
            std::string num = std::to_string(i);

            glm::vec3 color = white;
            if (i == 7) color = red;

            glUniform3fv(glGetUniformLocation(shader.Program, ("pointLights[" + num + "].position").c_str()),
                1, glm::value_ptr(pointLightPositions[i]));

            // Luz ambiental tenue
            glUniform3fv(glGetUniformLocation(shader.Program, ("pointLights[" + num + "].ambient").c_str()),
                1, glm::value_ptr(color * 0.1f));

            // Ajuste de intensidad para luces 003, 004 y 005 (indices 0, 1, 6)
            float diffuseIntensity = 2.0f;
            if (i == 0 || i == 1 || i == 6)
                diffuseIntensity = 0.6f; // mas suaves

            // Luz difusa
            glUniform3fv(glGetUniformLocation(shader.Program, ("pointLights[" + num + "].diffuse").c_str()),
                1, glm::value_ptr(color * diffuseIntensity));

            // Reflejo especular tambien reducido en esas luces
            float specularIntensity = (i == 0 || i == 1 || i == 6) ? 0.5f : 1.0f;
            glUniform3fv(glGetUniformLocation(shader.Program, ("pointLights[" + num + "].specular").c_str()),
                1, glm::value_ptr(color * specularIntensity));

            // Atenuacion (alcance de luz)
            glUniform1f(glGetUniformLocation(shader.Program, ("pointLights[" + num + "].constant").c_str()), 1.0f);
            glUniform1f(glGetUniformLocation(shader.Program, ("pointLights[" + num + "].linear").c_str()), 0.07f);
            glUniform1f(glGetUniformLocation(shader.Program, ("pointLights[" + num + "].quadratic").c_str()), 0.017f);
        }



        // Posicion de la camara
        glUniform3f(glGetUniformLocation(shader.Program, "viewPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


        // Draw the loaded model
        glm::mat4 model(1);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        backroom.Draw(shader);

        

        // Dibujar balón (
        shader.Use();
        glm::mat4 model2(1);

        if (playingKeyframes) {
            // Usar posición de keyframes
            model2 = glm::translate(model2, ballPosition);
            model2 = glm::rotate(model2, glm::radians(ballRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            model2 = glm::rotate(model2, glm::radians(ballRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            model2 = glm::rotate(model2, glm::radians(ballRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        }
        else {
            // Posición estática original
            model2 = glm::translate(model2, glm::vec3(-20.3f, -1.9f, -28.0f));
        }

        model2 = glm::scale(model2, glm::vec3(1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model2));
        balon.Draw(shader);



        // PERSONA 1
        if (person1AnimationActive && !person1AnimationPlayed) {
            float timeInAnimation = t - person1AnimationStartTime;
            float durationInSeconds = 843.0f / 30.0f;  // 843 frames a 30 FPS = 28.1 segundos 

            animacion.UpdateAnimation(person1AnimationStartTime + timeInAnimation);

            if (timeInAnimation >= durationInSeconds) {
                person1AnimationPlayed = true;
                person1AnimationActive = false;
              
                std::cout << "Animación persona 1 completada - volviendo al inicio\n";
            }
        }
      
        else {
            animacion.UpdateAnimation(0.0f);
        }

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
        if (person2AnimationActive && !person2AnimationPlayed) {
            float timeInAnimation = t - person2AnimationStartTime;
            float durationInSeconds = 843.0f / 30.0f;  // 843 frames a 30 FPS = 28.1 segundos

            animacion2.UpdateAnimation(person2AnimationStartTime + timeInAnimation);

            if (timeInAnimation >= durationInSeconds) {
                person2AnimationPlayed = true;
                person2AnimationActive = false;
                std::cout << "Animación persona 2 completada - volviendo al inicio\n";
            }
        }
        else {
            animacion2.UpdateAnimation(0.0f);
        }

        std::vector<glm::mat4> bones2;
        animacion2.GetBoneMatrices(bones2, 100);

        if (bonesLoc >= 0 && !bones2.empty())
            glUniformMatrix4fv(bonesLoc, (GLsizei)bones2.size(), GL_FALSE, &bones2[0][0][0]);

        // PERSONA 2
        glm::mat4 modelAnim2(1.0f);
        modelAnim2 = glm::translate(modelAnim2, glm::vec3(-13.0f, -2.0f, -60.0f)); // mismo Z que el otro
        modelAnim2 = glm::rotate(modelAnim2, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelAnim2 = glm::scale(modelAnim2, glm::vec3(0.035f));
        glUniformMatrix4fv(glGetUniformLocation(skinnedShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelAnim2));

        animacion2.Draw(skinnedShader);

       // JUGADOR - Solo animar cuando se active con B
if (animationActivated && !playerAnimationPlayed) {
    float timeInAnimation = t - playerAnimationTime;
    float durationInSeconds = 48.0f / 30.0f;  // 48 frames a 30 FPS = 1.6 segundos
    
    animacion3.UpdateAnimation(playerAnimationTime + timeInAnimation);

    // Verificar si la animación llegó al final
    if (timeInAnimation >= durationInSeconds) {
        playerAnimationPlayed = true;
        animationActivated = false;  // para desactivar
        std::cout << "Animación del jugador completada - volviendo al inicio\n";
    }
}
else {
    // Mantener en frame 0
    animacion3.UpdateAnimation(0.0f);
}

std::vector<glm::mat4> bones3;
animacion3.GetBoneMatrices(bones3, 100);

        if (bonesLoc >= 0 && !bones3.empty())
            glUniformMatrix4fv(bonesLoc, (GLsizei)bones3.size(), GL_FALSE, &bones3[0][0][0]);

        //JUGADOR
        glm::mat4 modelAnim3(1.0f);
        modelAnim3 = glm::translate(modelAnim3, glm::vec3(-20.0f, -2.0f, -30.0f)); 
        modelAnim3 = glm::scale(modelAnim3, glm::vec3(0.035f));
        glUniformMatrix4fv(glGetUniformLocation(skinnedShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelAnim3));

        animacion3.Draw(skinnedShader);

        // Swap the buffers
        glfwSwapBuffers(window);
    }
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
   
    // Tecla B - Iniciar animación pelota y jugador
    if (key == GLFW_KEY_B && action == GLFW_PRESS) {
        if (!animationActivated) {  // Solo si no está activada
            animationActivated = true;  // ← Activar todo
            playerAnimationTime = glfwGetTime();
            playerAnimationPlayed = false;
            ballAnimationTriggered = false;
            animationTime = 0.0f;
            currentKeyframe = 0;
            std::cout << "✓ Animación iniciada con tecla B\n";
        }
    }
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        // Resetear COMPLETAMENTE
        animationActivated = false;  
        playerAnimationPlayed = false;
        playingKeyframes = false;
        ballAnimationTriggered = false;
        animationTime = 0.0f;
        currentKeyframe = 0;
        ballPosition = glm::vec3(-20.0f, -1.9f, -28.0f);
        ballRotation = glm::vec3(0.0f, 0.0f, 0.0f);
        // Agregar reset de personas:
        person1AnimationActive = false;
        person2AnimationActive = false;
        person1AnimationPlayed = false;
        person2AnimationPlayed = false;
        person1AnimationStartTime = 0.0f;
        person2AnimationStartTime = 0.0f;

        std::cout << "✓ Todo reseteado - Presiona B para jugador/pelota, T para personas\n";
    }
    // Tecla T - Iniciar animaciones de las personas bailando
    if (key == GLFW_KEY_T && action == GLFW_PRESS) {
        if (!person1AnimationActive) {  // Solo si no están activadas
            person1AnimationActive = true;
            person2AnimationActive = true;
            person1AnimationStartTime = glfwGetTime();
            person2AnimationStartTime = glfwGetTime();
            person1AnimationPlayed = false;
            person2AnimationPlayed = false;
            std::cout << "✓ Animaciones de personas iniciadas\n";
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