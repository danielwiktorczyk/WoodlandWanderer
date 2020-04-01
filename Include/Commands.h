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

	static void panCamera(GLFWwindow* window, float& camHorAng, const double& dx, const float& camAngSpeed);
	static void tiltCamera(GLFWwindow* window, float& camVertAng, const double& dy, const float& camAngSpeed);
	static void zoomCamera(GLFWwindow* window, 
		float& currentFOV,
		const double& dy, 
		glm::mat4& projMatrix, 
		const int& colorShader, 
		std::function<void(int, glm::mat4)> setProjMatrix);

	static void setWorldRotation(GLFWwindow* window, float& rotYaxis, float& rotXaxis);
};