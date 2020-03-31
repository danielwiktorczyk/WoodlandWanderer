#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Commands {
public:
	static void areTexturesToggled(GLFWwindow* window, bool& canToggle, bool& texOn);
	static void closeWindow(GLFWwindow* window);
	static void setRenderingMode(GLFWwindow* window);
	static void panCamera(GLFWwindow* window, float& camHorAng, const double& dx, const float& camAngSpeed);
	static void tiltCamera(GLFWwindow* window, float& camVertAng, const double& dy, const float& camAngSpeed);

	static void setWorldRotation(GLFWwindow* window, float& rotYaxis, float& rotXaxis);
};