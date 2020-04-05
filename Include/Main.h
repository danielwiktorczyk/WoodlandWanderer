#pragma once
#include <iostream>
#include <list>
#include <cstdlib>   // Random
#include <ctime>     // Time for Random Seeding

#define GLEW_STATIC 1 
#include <GL/glew.h>    

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <fstream>
#include <sstream>
#include "../Include/OBJloader.h"  //For loading .obj files
#include "../Include/Snowman.h"
#include "../Include/Commands.h"
#include "../Include/Colours.h"
#include "../Include/Model.h"
#include "../Include/CollidableModel.h"
#include "../Include/NonCollidableModel.h"
#include "../Include/Forest.h"
#include "../Include/Acre.h"
#include "../Include/Tile.h"
#include "../Include/Assets.h"
 
int compileAndLinkShaders(const std::string vertexShaderSource, const std::string fragmentShaderSource);

GLuint setupModelVBO_OLD(std::string path, int& vertexCount);

void setProjectionMatrix(int shaderProgram, glm::mat4 projectionMatrix);
void setViewMatrix(int shaderProgram, glm::mat4 viewMatrix);
void setWorldMatrix(int shaderProgram, glm::mat4 worldMatrix);
void setWorldRotationMatrix(int shaderProgram, glm::mat4 worldRotationMatrix);

void sendViewMatrixToShader(const glm::vec3& camPos,
							const glm::vec3& camLookAt,
							const glm::vec3 camUp,
							const int& colorShader,
							const GLuint& viewLocationColor);

void sendWorldRotationMatrixToShader(glm::mat4& worldRotationMatrix, const float& rotYaxis, const float& rotXaxis, const int& colorShader);

void setCameraVariables(GLFWwindow* window, 
						double& mousePosX, 
						double& mousePosY, 
						double& lastMousePosX, 
						double& lastMousePosY, 
						float& camHorAngle,
						float& camVertAngle,
						glm::vec3& cameraLookAt,
						glm::vec3& cameraSideVector);

///////////////////////////////////////////////////////////////////
////////////////////////// Variables //////////////////////////////
///////////////////////////////////////////////////////////////////

double lastMousePosX, lastMousePosY;

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