#include "../Include/Commands.h"

/**
* Checks to see if the user pressed enter.
* If they did, the program exits.
*/
void Commands::closeWindow(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

/**
* Checks to see which rendering mode is selected by the user.
*/
void Commands::setRenderingMode(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	}
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

/**
* Set the world orientation
*/
void Commands::setWorldRotation(GLFWwindow* window, float& rotYaxis, float& rotXaxis) {
	float worldRotationSpeed = 0.005f;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		rotYaxis += worldRotationSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		rotYaxis -= worldRotationSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		rotXaxis += worldRotationSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		rotXaxis -= worldRotationSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS) {
		rotXaxis = 0.0f;
		rotYaxis = 0.0f;
	}
}
