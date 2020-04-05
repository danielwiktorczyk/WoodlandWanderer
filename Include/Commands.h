#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>

class Commands {
public:
	static void closeWindow(GLFWwindow* window);
	static void setRenderingMode(GLFWwindow* window);

	static void setWorldRotation(GLFWwindow* window, float& rotYaxis, float& rotXaxis);
};