#pragma once

#include <array>
#include <cstdint>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Commands {
public:
	static void processCloseWindow(GLFWwindow* window);
	static void processCameraMovement(GLFWwindow* window, glm::vec3& cameraPos, glm::vec3& cameraFront, glm::vec3& cameraUp, float& deltaTime);
};

