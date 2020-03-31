#pragma once
#include <iostream>
#include <list>
#include <cstdlib>   // Random
#include <ctime>     // Time for Random Seeding

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // cross-platform interface for creating a graphical context,
// initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <fstream>
#include <sstream>
#include "../Include/OBJloader.h"  //For loading .obj files
#include "../Include/Snowman.h"
#include "../Include/Commands.h"
#include "../Include/Colours.h"
#include "../Include/LoadedObject.h"

// for textures: 
#define STB_IMAGE_IMPLEMENTATION
#include "../Include/stb_image.h"

//const char* getVertexShaderSource();
//const char* getFragmentShaderSource();
//const char* getTexturedVertexShaderSource();
//const char* getTexturedFragmentShaderSource();

int compileAndLinkShaders(const std::string vertexShaderSource, const std::string fragmentShaderSource);

GLuint setupModelVBO(std::string path, int& vertexCount);
GLuint loadTexture(const char* filename);

void setProjectionMatrix(int shaderProgram, glm::mat4 projectionMatrix);
void setViewMatrix(int shaderProgram, glm::mat4 viewMatrix);
void setWorldMatrix(int shaderProgram, glm::mat4 worldMatrix);
void setWorldRotationMatrix(int shaderProgram, glm::mat4 worldRotationMatrix);

void sendViewMatrixToShader(const glm::vec3& camPos,
							const glm::vec3& camLookAt,
							const glm::vec3 camUp,
							const int& colorShader,
							const int& textShader,
							const GLuint& viewLocationColor,
							const GLuint& viewLocationTexture);

void sendWorldRotationMatrixToShader(glm::mat4& worldRotationMatrix,
									 const float& rotYaxis,
									 const float& rotXaxis,
									 const int& colorShader,
									 const int& textShader);

void setCameraVariables(GLFWwindow* window, 
						double& mousePosX, 
						double& mousePosY, 
						double& lastMousePosX, 
						double& lastMousePosY, 
						float& camHorAngle,
						float& camVertAngle,
						glm::vec3& cameraLookAt,
						glm::vec3& cameraSideVector);

void bind(int& currentShader, GLuint& VAO);

///////////////////////////////////////////////////////////////////
////////////////////////// Variables //////////////////////////////
///////////////////////////////////////////////////////////////////

// Camera parameters for view transform
glm::vec3 cameraPosition(3.0f, 5.0f, 25.0f);
glm::vec3 cameraLookAt(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);

// Other camera parameters
float cameraSpeed = 0.05f;
float cameraHorizontalAngle = 90.0f;
float cameraVerticalAngle = 0.0f;
glm::vec3 cameraSideVector = glm::vec3(1.0f);

// Set projection matrix for shader
float currentFOV = 70.0f;
glm::mat4 projectionMatrix = glm::perspective(currentFOV, 1024.0f / 768.0f, 0.01f, 100.0f);

// Set initial view matrix
glm::mat4 viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);

// Texturing toggle
bool texturing = false;
bool canToggleTexturing = true;

// World rotation
glm::mat4 worldRotationMatrix = glm::mat4(1.0f);
float worldRotationAboutXAxis = 0.0f;
float worldRotationAboutYAxis = 0.0f;

// Controlling increment helper variables
bool canScaleIncrement = true;
bool canRotateIncrement = true;
bool canMoveIncrement = true;
bool canRandomPlacement = true;

//Camera
double mousePosX, mousePosY;
double dx, dy;
float theta, phi;
constexpr float cameraAngularSpeed = glm::radians(5.0f);