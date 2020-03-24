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
	static void processInput(GLFWwindow* window);
};

