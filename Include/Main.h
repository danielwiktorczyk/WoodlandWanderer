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
#include "../Include/OBJloader.h"  //For loading .obj files
#include "../Include/Snowman.h"
#include "../Include/Commands.h"
#include "../Include/Colours.h"

// for textures: 
#define STB_IMAGE_IMPLEMENTATION
#include "../Include/stb_image.h"
#define M_PI 3.1415926535897932384626433832795

const char* getVertexShaderSource();
const char* getFragmentShaderSource();
const char* getTexturedVertexShaderSource();
const char* getTexturedFragmentShaderSource();

int compileAndLinkShaders(const char* vertexShaderSource, const char* fragmentShaderSource);

GLuint setupModelVBO(std::string path, int& vertexCount);
GLuint loadTexture(const char* filename);

void setProjectionMatrix(int shaderProgram, glm::mat4 projectionMatrix);
void setViewMatrix(int shaderProgram, glm::mat4 viewMatrix);
void setWorldMatrix(int shaderProgram, glm::mat4 worldMatrix);
void setWorldRotationMatrix(int shaderProgram, glm::mat4 worldRotationMatrix);
