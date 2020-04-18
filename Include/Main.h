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
#include "../Include/Parameters.h"
#include "../Include/Camera.h";
 
int compileAndLinkShaders(const std::string vertexShaderSource, const std::string fragmentShaderSource);

GLuint setupModelVBO_OLD(std::string path, int& vertexCount);

void setProjectionMatrix(const int& shaderProgram);
void setViewMatrix(const int& shaderProgram);

void mouseCallback(GLFWwindow* window, double xPos, double yPos);

///////////////////////////////////////////////////////////////////
////////////////////////// Variables //////////////////////////////
///////////////////////////////////////////////////////////////////

// Camera parameters for view transform
glm::vec3 cameraPosition(3.0f, 5.0f, 25.0f);
glm::vec3 cameraLookAt(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);

// Other camera parameters
bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = (HD ? WindowWidthHD : WindowWidth)/ 2.0;
float lastY = 768 / 2.0;
float fov = 45.0f;
float lastFrame = 0.0f;

glm::mat4 projectionMatrix = glm::perspective(fov, (HD ? WindowWidthHD : WindowWidth) / (HD ? WindowHeightHD : WindowHeight), 0.01f, 100.0f);
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