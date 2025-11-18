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
// Cuadro 1
glm::vec3 cuadro1Pos = convertBlenderToOpenGL(-0.94665f, 33.938f, 4.0745f);
glm::vec3 cuadro1Rot = glm::vec3(glm::radians(90.0f), glm::radians(0.0f), glm::radians(-180.0f));
glm::vec3 cuadro1Scale = glm::vec3(1.000f, 1.084f, 1.339f);

// Cuadro 2
glm::vec3 cuadro2Pos = convertBlenderToOpenGL(-11.172f, 29.158f, 3.5151f);
glm::vec3 cuadro2Rot = glm::vec3(glm::radians(90.0f), glm::radians(0.0f), glm::radians(-270.0f));
glm::vec3 cuadro2Scale = glm::vec3(1.000f, 1.089f, 1.312f);


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

float waterWaveTime = 0.0f;
GLuint waterVAO, waterVBO;

// Variables para animación de cuadros
float cuadro1AnimTime = 0.0f;
float cuadro2AnimTime = 0.0f;
bool animCuadrosActiva = false;

// Variables para animación de luces
float lightAnimationTime = 0.0f;
bool lightAnimationActive = false;
float beatInterval = 3.3f; 
float audioLevel = 0.0f;

//Variable para neblina
bool fogActive = false;

enum RoomType {
    COLD_ROOM,  // Tonos fríos: azul, verde menta, blanco
    WARM_ROOM,  // Tonos cálidos: amarillo, naranja
    RED_LIGHT   // Luz roja especial
};

// Función para determinar qué tipo de luz es cada índice
RoomType getLightRoomType(int lightIndex) {
    // Point.003 (0), Point.005 (1), Point.004 (6) = FRÍAS
    if (lightIndex == 0 || lightIndex == 1 || lightIndex == 6) {
        return COLD_ROOM;
    }

    // Point.001 (2), Point (3), Point.002 (4), Light (5) = CÁLIDAS
    if (lightIndex == 2 || lightIndex == 3 || lightIndex == 4 || lightIndex == 5) {
        return WARM_ROOM;
    }

    // Índice 7 = Luz roja
    if (lightIndex == 7) {
        return RED_LIGHT;
    }

    return COLD_ROOM; // Default
}

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

// Murciélagos
glm::vec3 batPosition = glm::vec3(-10.3f, 3.0f, -9.0f);  // Cerca del balón
float batScale = 0.0009f;
float batAnimationStartTime = 0.0f;
bool batsActive = true;  // que vuelen desde el inicio


// Variables para el monstruo
bool monstruoAnimationActive = false;
int monstruoAnim = 0;  // Estado de la maquina
glm::vec3 monstruoPos = convertBlenderToOpenGL(-32.251f, 3.1166f, -2.0f);  // Posicion inicial
float monstruoRot = 0.0f;  // Rotacion del monstruo
glm::vec3 puntoAzul = convertBlenderToOpenGL(-31.173f, 16.083f, -2.0f);  // Primer destino
glm::vec3 puntoIntermedio = convertBlenderToOpenGL(-24.347f, 17.35f, -2.0f);  // Segundo destino
glm::vec3 destinoFinal = convertBlenderToOpenGL(-20.483f, 39.0f, -2.0f);  // Destino final 
float monstruoAnimationStartTime = 0.0f;

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
        std::cout << "Pelota disparada en frame : " << currentFrame << "\n";
    }

    // Resetear trigger
    if (currentFrame < 12.0f || currentFrame > 14.0f) {
        ballAnimationTriggered = false;
    }
}

//Funcion para crear el cubo
void setupWaterCube() {
    float waterVertices[] = {
        // Posiciones          Normales
        // Cara frontal
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        // Cara trasera
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        // Cara izquierda
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

        // Cara derecha
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

         // Cara inferior
         -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
          0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
          0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
          0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

         // Cara superior
         -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
          0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
          0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
          0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f
    };

    glGenVertexArrays(1, &waterVAO);
    glGenBuffers(1, &waterVBO);

    glBindVertexArray(waterVAO);
    glBindBuffer(GL_ARRAY_BUFFER, waterVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(waterVertices), waterVertices, GL_STATIC_DRAW);

    // Posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normales
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}
//Funcion para dibujar el agua
void drawWaterCube(Shader& shader, glm::mat4 projection, glm::mat4 view, float time) {
    shader.Use();

    // Habilitar transparencia
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Ver interior del cubo
    glDisable(GL_CULL_FACE);

    // Indicar que NO use texturas
    glUniform1i(glGetUniformLocation(shader.Program, "useTexture"), 0); // 0 = false

    // Color del agua (azul translúcido)
    glUniform3f(glGetUniformLocation(shader.Program, "objectColor"), 0.2f, 0.5f, 0.9f);

    // Posición del cubo (convertida de Blender a OpenGL)
    glm::mat4 model(1.0f);
    glm::vec3 waterPos = convertBlenderToOpenGL(-31.786f, 46.235f, -0.74377f);
    model = glm::translate(model, waterPos);

    // Escala (convertida: X, Z de Blender, Y de Blender)
    model = glm::scale(model, glm::vec3(18.8f, 2.42f, 13.2f));

    // Animación de ondas (rotación oscilatoria)
    float waveX = sin(time * 2.5f) * 0.015f;
    float waveZ = cos(time * 1.8f) * 0.015f;
    model = glm::rotate(model, waveX, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, waveZ, glm::vec3(0.0f, 0.0f, 1.0f));

    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // Material brillante del agua
    glUniform1f(glGetUniformLocation(shader.Program, "material.shininess"), 64.0f);

    // Posición de la cámara para efectos
    glUniform3f(glGetUniformLocation(shader.Program, "viewPos"),
        camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
    //CONFIGURACION DE LUCES PARA EL AGUA

   // Luz direccional con tono azul brillante
    glUniform3f(glGetUniformLocation(shader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
    glUniform3f(glGetUniformLocation(shader.Program, "dirLight.ambient"), 0.3f, 0.4f, 0.7f);  // Azul ambiente
    glUniform3f(glGetUniformLocation(shader.Program, "dirLight.diffuse"), 0.4f, 0.6f, 1.0f);   // Azul difuso
    glUniform3f(glGetUniformLocation(shader.Program, "dirLight.specular"), 0.6f, 0.8f, 1.0f);  // Azul especular

    // Configurar point lights con efecto azul agua
    for (int i = 0; i < 8; i++) {
        std::string num = std::to_string(i);

        glUniform3fv(glGetUniformLocation(shader.Program, ("pointLights[" + num + "].position").c_str()),
            1, glm::value_ptr(pointLightPositions[i]));
        // Color azulado para las luces
        glm::vec3 waterColor = glm::vec3(0.3f, 0.5f, 1.0f);

        glUniform3fv(glGetUniformLocation(shader.Program, ("pointLights[" + num + "].ambient").c_str()),
            1, glm::value_ptr(waterColor * 0.4f));

        glUniform3fv(glGetUniformLocation(shader.Program, ("pointLights[" + num + "].diffuse").c_str()),
            1, glm::value_ptr(waterColor * 0.8f));

        glUniform3fv(glGetUniformLocation(shader.Program, ("pointLights[" + num + "].specular").c_str()),
            1, glm::value_ptr(waterColor * 1.2f));

        glUniform1f(glGetUniformLocation(shader.Program, ("pointLights[" + num + "].constant").c_str()), 1.0f);
        glUniform1f(glGetUniformLocation(shader.Program, ("pointLights[" + num + "].linear").c_str()), 0.07f);
        glUniform1f(glGetUniformLocation(shader.Program, ("pointLights[" + num + "].quadratic").c_str()), 0.017f);
    }

    glBindVertexArray(waterVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    // Restaurar estados
    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);

    //  Reactiva texturas para otros objetos
    glUniform1i(glGetUniformLocation(shader.Program, "useTexture"), 1); // 1 = true
}

void audioCallback(void* userdata, Uint8* stream, int len) {
    float sum = 0.0f;

    for (int i = 0; i < len; i++) {
        float sample = ((float)stream[i] - 128.0f) / 128.0f;
        sum += fabs(sample);
    }

    audioLevel = sum / len;
}
float getLightIntensity(float time, int lightIndex) {
    float base = 0.3f;

    float audioResponse = audioLevel * 1.0f;

    float phase = 0.4f * lightIndex;
    float wave = 0.6f + 0.4f * sin(time * 2.0f + phase);

    float result = base + audioResponse * wave;

    if (result < base) result = base;
    if (result > 3.0f) result = 3.0f;

    return result;
}

// Función para obtener color pulsante
glm::vec3 getPulsingColor(float time, int i) {
    float intensity = getLightIntensity(time, i);
    RoomType roomType = getLightRoomType(i);

    if (roomType == RED_LIGHT) {
        // Luz roja
        return glm::vec3(intensity, 0.0f, 0.0f);
    }
    else if (roomType == COLD_ROOM) {
        // Tonos FRÍOS: azul, verde menta, blanco
        float phase = time + i;
        float r = intensity * (0.4f + 0.3f * sin(phase * 1.2f));      // Poco rojo (blanco)
        float g = intensity * (0.7f + 0.3f * sin(phase * 1.5f));      // Verde menta
        float b = intensity * (0.9f + 0.1f * sin(phase * 1.8f));      // Azul

        return glm::vec3(r, g, b);
    }
    else { // WARM_ROOM
        // Tonos CÁLIDOS: amarillo, naranja, toque oscuro
        float phase = time + i;
        float r = intensity * (0.9f + 0.1f * sin(phase * 1.3f));      // Rojo
        float g = intensity * (0.5f + 0.3f * sin(phase * 1.6f));      // Amarillo/naranja
        float b = intensity * (0.1f + 0.1f * sin(phase * 1.0f));      // Azul (oscuro)

        return glm::vec3(r, g, b);
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

    Mix_SetPostMix(audioCallback, nullptr);

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
    Model monstruo((char*)"Models/monstruo.fbx");
    Model techo((char*)"Models/Techo/techo.obj");
   // Model cuadro1((char*)"Models/paintingMov.fbx");
   // Model cuadro2((char*)"Models/pinturaAnimada2.fbx");
    Model muercielago((char*)"Models/bat_animation_fly.fbx");



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

        if (lightAnimationActive && musicOn) {
            lightAnimationTime += deltaTime;
        }

        if (animCuadrosActiva) {
            cuadro1AnimTime += deltaTime;
            cuadro2AnimTime += deltaTime * 1.5f;
        }


        // Check and call events
        glfwPollEvents();
        DoMovement();

        double t = glfwGetTime();
        CheckAndTriggerBallAnimation(t);
        UpdateBallAnimation(t);


        // Clear the colorbuffer
        glClearColor(0.02f, 0.02f, 0.02f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightingShader.Use();
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);


        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Luz direccional 
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f); // dirección de la luz
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.01f, 0.01f, 0.01f); // aumento de luz ambiental
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.8f, 0.8f, 0.8f); // aumento de luz difusa
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f); // mayor brillo especular

        glm::vec3 white = glm::vec3(1.0f);
        glm::vec3 red = glm::vec3(1.0f, 0.0f, 0.0f);

        for (int i = 0; i < 8; i++) {
            std::string num = std::to_string(i);

            glm::vec3 color;
            float intensity;

            if (lightAnimationActive && musicOn) {
                // Modo animación - luces reaccionando al audio
                color = getPulsingColor(lightAnimationTime, i);
                intensity = getLightIntensity(lightAnimationTime, i);

                glUniform3fv(glGetUniformLocation(lightingShader.Program, ("pointLights[" + num + "].position").c_str()),
                    1, glm::value_ptr(pointLightPositions[i]));

                glUniform3fv(glGetUniformLocation(lightingShader.Program, ("pointLights[" + num + "].ambient").c_str()),
                    1, glm::value_ptr(color * 0.3f));

                glUniform3fv(glGetUniformLocation(lightingShader.Program, ("pointLights[" + num + "].diffuse").c_str()),
                    1, glm::value_ptr(color * 0.7f * intensity));

                glUniform3fv(glGetUniformLocation(lightingShader.Program, ("pointLights[" + num + "].specular").c_str()),
                    1, glm::value_ptr(color * intensity));
            }
            else {

            // Modo normal - luces estáticas según tipo de sala
            RoomType roomType = getLightRoomType(i);

            if (roomType == RED_LIGHT) {
                color = red;  // Rojo
            }
            else if (roomType == COLD_ROOM) {
                color = glm::vec3(0.6f, 0.9f, 1.0f);  // Azul claro/verde menta
            }
            else { // WARM_ROOM
                color = glm::vec3(1.0f, 0.7f, 0.3f);  // Naranja cálido
            }
                float diffuseIntensity = (i == 0 || i == 1 || i == 6) ? 0.1f : 2.0f;

                glUniform3fv(glGetUniformLocation(lightingShader.Program, ("pointLights[" + num + "].position").c_str()),
                    1, glm::value_ptr(pointLightPositions[i]));

                glUniform3fv(glGetUniformLocation(lightingShader.Program, ("pointLights[" + num + "].ambient").c_str()),
                    1, glm::value_ptr(color * 0.1f));

                glUniform3fv(glGetUniformLocation(lightingShader.Program, ("pointLights[" + num + "].diffuse").c_str()),
                    1, glm::value_ptr(color * diffuseIntensity));

                float specularIntensity = (i == 0 || i == 1 || i == 6) ? 0.5f : 1.0f;
                glUniform3fv(glGetUniformLocation(lightingShader.Program, ("pointLights[" + num + "].specular").c_str()),
                    1, glm::value_ptr(color * specularIntensity));
            }

            // Atenuación (igual para ambos modos)
            glUniform1f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + num + "].constant").c_str()), 1.0f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + num + "].linear").c_str()), 0.07f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + num + "].quadratic").c_str()), 0.017f);
        }

        // Activar / desactivar neblina
            glUniform1i(glGetUniformLocation(lightingShader.Program, "useFog"), fogActive ? 1 : 0);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "fogColor"),
            0.6f, 0.6f, 0.65f); // Color gris neblinoso
        glUniform1f(glGetUniformLocation(lightingShader.Program, "fogDensity"),
            0.06f);

        // Tiempo para animación de neblina
        glUniform1f(glGetUniformLocation(lightingShader.Program, "fogTime"),
            currentFrame);

        // Límites del edificio
        glUniform3f(glGetUniformLocation(lightingShader.Program, "buildingMin"),
            -45.0f, -3.0f, -70.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "buildingMax"),
            5.0f, 15.0f, 5.0f);

        // Posicion de la camara
        glUniform3f(glGetUniformLocation(lightingShader.Program, "viewPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        lightingShader.Use();
        glm::mat4 modelTecho(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelTecho));
        techo.Draw(lightingShader);

        // Draw the loaded model
        glm::mat4 model(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        backroom.Draw(lightingShader);
       
        //Dibujar cuadros
        
        //Cuadro 1
       /* {
            glm::mat4 m1(1.0f);
            m1 = glm::translate(m1, cuadro1Pos);

            if (animCuadrosActiva) {
                float wobble = sin(cuadro1AnimTime * 2.0f) * 0.1f;
                m1 = glm::rotate(m1, wobble, glm::vec3(0, 1, 0));
            }

            m1 = glm::rotate(m1, cuadro1Rot.x, glm::vec3(1, 0, 0));
            m1 = glm::rotate(m1, cuadro1Rot.y, glm::vec3(0, 1, 0));
            m1 = glm::rotate(m1, cuadro1Rot.z, glm::vec3(0, 0, 1));

            m1 = glm::scale(m1, cuadro1Scale);

            glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(m1));
            cuadro1.Draw(lightingShader);
        }*/

		//Cuadro 2
       /* {
            glm::mat4 m2(1.0f);
            m2 = glm::translate(m2, cuadro2Pos);

            if (animCuadrosActiva) {
                float shake = sin(cuadro2AnimTime * 6.0f) * 0.05f;
                m2 = glm::translate(m2, glm::vec3(0, shake, 0));
            }

            m2 = glm::rotate(m2, cuadro2Rot.x, glm::vec3(1, 0, 0));
            m2 = glm::rotate(m2, cuadro2Rot.y, glm::vec3(0, 1, 0));
            m2 = glm::rotate(m2, cuadro2Rot.z, glm::vec3(0, 0, 1));

            m2 = glm::scale(m2, cuadro2Scale);

            glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(m2));
            cuadro2.Draw(lightingShader);
        }*/

        // Actualizar tiempo de animación del agua
            waterWaveTime = currentFrame;

        // Dibujar cubo de agua 
        drawWaterCube(lightingShader, projection, view, waterWaveTime);

        // Después de dibujar el agua, restaurar las luces originales para los demás objetos
        lightingShader.Use();

        // Restaurar luz direccional original
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.01f, 0.01f, 0.01f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.03f, 0.03f, 0.03f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.02f, 0.02f, 0.02f);

        // Restaurar point lights originales
        for (int i = 0; i < 8; i++) {
            std::string num = std::to_string(i);
            glm::vec3 color = (i == 7) ? red : white;

            glUniform3fv(glGetUniformLocation(lightingShader.Program, ("pointLights[" + num + "].ambient").c_str()),
                1, glm::value_ptr(color * 0.1f));

            float diffuseIntensity = (i == 0 || i == 1 || i == 6) ? 0.6f : 2.0f;
            glUniform3fv(glGetUniformLocation(lightingShader.Program, ("pointLights[" + num + "].diffuse").c_str()),
                1, glm::value_ptr(color * diffuseIntensity));

            float specularIntensity = (i == 0 || i == 1 || i == 6) ? 0.5f : 1.0f;
            glUniform3fv(glGetUniformLocation(lightingShader.Program, ("pointLights[" + num + "].specular").c_str()),
                1, glm::value_ptr(color * specularIntensity));
        }

        // Dibujar balón (
        lightingShader.Use();
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
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model2));
        balon.Draw(lightingShader);


        // PERSONA 1
        if (person1AnimationActive && !person1AnimationPlayed) {
            float timeInAnimation = t - person1AnimationStartTime;
            float durationInSeconds = 843.0f / 30.0f;  // 843 frames a 30 FPS = 28.1 segundos 

            animacion.UpdateAnimation(person1AnimationStartTime + timeInAnimation);

            if (timeInAnimation >= durationInSeconds) {
                person1AnimationPlayed = true;
                person1AnimationActive = false;

                std::cout << "Animacion persona 1 completada - volviendo al inicio\n";
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
        modelAnim1 = glm::translate(modelAnim1, glm::vec3(-20.0f, -2.0f, -60.0f)); 
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
                std::cout << "Animacion persona 2 completada - volviendo al inicio\n";
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
                std::cout << "Animacion del jugador completada - volviendo al inicio\n";
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
        modelAnim3 = glm::scale(modelAnim3, glm::vec3(0.03f));
        glUniformMatrix4fv(glGetUniformLocation(skinnedShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelAnim3));
        animacion3.Draw(skinnedShader);

        // MONSTRUO - Maquina de estados
        if (monstruoAnim >= 1 && monstruoAnim <= 6) {  // Animar en estados 1-5
            // Animar caminar (loop de la animacion)
            float timeInAnimation = t - monstruoAnimationStartTime;
            float animDuration = 1.5f;
            float loopTime = fmod(timeInAnimation, animDuration);
            monstruo.UpdateAnimation(loopTime);
        }
        else {
            monstruo.UpdateAnimation(0.0f);  // Pose inicial
        }

        // E0: Idle - Esperando tecla Y

        // E1: Rotar hacia punto azul (pared)
        if (monstruoAnim == 1) {
            glm::vec3 direccion = puntoAzul - monstruoPos;
            float anguloObjetivo = atan2(direccion.x, direccion.z) * 180.0f / 3.14159f;

            if (abs(monstruoRot - anguloObjetivo) > 1.0f) {
                monstruoRot += (anguloObjetivo - monstruoRot) * 0.02f;
            }
            else {
                monstruoRot = anguloObjetivo;
                monstruoAnim = 2;
            }
        }

        // E2: Caminar hacia punto azul
        else if (monstruoAnim == 2) {
            glm::vec3 direccion = glm::normalize(puntoAzul - monstruoPos);
            float velocidad = 0.01f;
            monstruoPos += direccion * velocidad;

            float distancia = glm::length(puntoAzul - monstruoPos);
            if (distancia < 1.5f) {
                monstruoAnim = 3;
                std::cout << "Monstruo llego a la pared\n";
            }
        }

        // E3: Girar a la derecha hacia punto intermedio
        else if (monstruoAnim == 3) {
            glm::vec3 direccion = puntoIntermedio - monstruoPos;
            float anguloObjetivo = atan2(direccion.x, direccion.z) * 180.0f / 3.14159f;

            if (abs(monstruoRot - anguloObjetivo) > 1.0f) {
                monstruoRot += (anguloObjetivo - monstruoRot) * 0.02f;
            }
            else {
                monstruoRot = anguloObjetivo;
                monstruoAnim = 4;
            }
        }

        // E4: Caminar hacia punto intermedio
        else if (monstruoAnim == 4) {
            glm::vec3 direccion = glm::normalize(puntoIntermedio - monstruoPos);
            float velocidad = 0.01f;
            monstruoPos += direccion * velocidad;

            float distancia = glm::length(puntoIntermedio - monstruoPos);
            if (distancia < 1.5f) {
                monstruoAnim = 5;
                std::cout << "Monstruo llego al punto intermedio\n";
            }
        }

        // E5: Girar hacia destino final (entrada)
        else if (monstruoAnim == 5) {
            glm::vec3 direccion = destinoFinal - monstruoPos;
            float anguloObjetivo = atan2(direccion.x, direccion.z) * 180.0f / 3.14159f;

            if (abs(monstruoRot - anguloObjetivo) > 1.0f) {
                monstruoRot += (anguloObjetivo - monstruoRot) * 0.02f;
            }
            else {
                monstruoRot = anguloObjetivo;
                monstruoAnim = 6;
            }
        }

        // E6: Caminar hacia destino final
        else if (monstruoAnim == 6) {
            glm::vec3 direccion = glm::normalize(destinoFinal - monstruoPos);
            float velocidad = 0.01f;
            monstruoPos += direccion * velocidad;

            float distancia = glm::length(destinoFinal - monstruoPos);
            if (distancia < 0.5f) {
                monstruoAnim = 7;
                std::cout << "Monstruo llego al destino final\n";
            }
        }

        // E7: Llegada (estatico)

        // Dibujar el monstruo
        std::vector<glm::mat4> bonesMonstruo;
        monstruo.GetBoneMatrices(bonesMonstruo, 100);

        if (bonesLoc >= 0 && !bonesMonstruo.empty())
            glUniformMatrix4fv(bonesLoc, (GLsizei)bonesMonstruo.size(), GL_FALSE, &bonesMonstruo[0][0][0]);

        glm::mat4 modelMonstruo(1.0f);
        modelMonstruo = glm::translate(modelMonstruo, monstruoPos);
        modelMonstruo = glm::rotate(modelMonstruo, glm::radians(monstruoRot), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMonstruo = glm::scale(modelMonstruo, glm::vec3(0.028f));
        glUniformMatrix4fv(glGetUniformLocation(skinnedShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelMonstruo));
        monstruo.Draw(skinnedShader);

    
        // MURCIÉLAGOS
        if (batsActive) {
            float batTime = fmod(currentFrame - batAnimationStartTime, 1.2f);
            muercielago.UpdateAnimation(batTime);

            std::vector<glm::mat4> bonesBat;
            muercielago.GetBoneMatrices(bonesBat, 100);

            GLint bonesLocBat = glGetUniformLocation(skinnedShader.Program, "bones");
            if (bonesLocBat >= 0 && !bonesBat.empty())
                glUniformMatrix4fv(bonesLocBat, (GLsizei)bonesBat.size(), GL_FALSE, &bonesBat[0][0][0]);

            glm::mat4 modelBat(1.0f);
            modelBat = glm::translate(modelBat, batPosition);
            modelBat = glm::rotate(modelBat, glm::radians(-40.0f), glm::vec3(0, 1, 0));
            modelBat = glm::scale(modelBat, glm::vec3(batScale));

            glUniformMatrix4fv(glGetUniformLocation(skinnedShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelBat));
            muercielago.Draw(skinnedShader);
        }


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

        setupWaterCube();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVAO);

    glDeleteVertexArrays(1, &waterVAO);
    glDeleteBuffers(1, &waterVBO);

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
            std::cout << "Animacion iniciada con tecla B\n";
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

        std::cout << "Todo reseteado - Presiona B para jugador/pelota, T para personas\n";
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
            std::cout << "Animaciones de personas iniciadas\n";
        }
    }
    //  Tecla L - Iniciar animaciones de luces
    if (key == GLFW_KEY_L && action == GLFW_PRESS) {
        lightAnimationActive = !lightAnimationActive;
        if (lightAnimationActive) {
            lightAnimationTime = 0.0f;
            std::cout << "Animacion de luces ACTIVADA\n";
        }
        else {
            std::cout << "Animacion de luces DESACTIVADA\n";
        }
    }

    // Tecla Y - Iniciar animacion del monstruo
    if (key == GLFW_KEY_Y && action == GLFW_PRESS) {
        if (monstruoAnim == 0) {  // Solo si esta en idle
            monstruoAnim = 1;  // Iniciar maquina de estados
            monstruoAnimationStartTime = glfwGetTime();
            std::cout << "Animacion de monstruo iniciada\n";
        }
    }

    // Tecla I - Reset solo del monstruo
    if (key == GLFW_KEY_I && action == GLFW_PRESS) {
        monstruoAnim = 0;
        monstruoPos = convertBlenderToOpenGL(-32.251f, 3.1166f, -2.0f);
        monstruoRot = 0.0f;
        std::cout << "Monstruo reseteado\n";
    }
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        animCuadrosActiva = !animCuadrosActiva;
        std::cout << "Animacion de cuadros: "
            << (animCuadrosActiva ? "ON" : "OFF") << "\n";
    }
    // Tecla U - Pausa la animación de murciélagos
    if (key == GLFW_KEY_U && action == GLFW_PRESS) {
        batsActive = !batsActive;
        if (batsActive)
            batAnimationStartTime = glfwGetTime();
    }
    // Tecla F - Activar y desactivar neblina
    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        fogActive = !fogActive;
        std::cout << "Niebla: " << (fogActive ? "ON" : "OFF") << "\n";
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
// Fin del proyecto final