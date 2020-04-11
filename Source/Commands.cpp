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
* Move the camera in the world
*/
void Commands::processCameraDirection(GLFWwindow* window, glm::vec3& cameraPos, glm::vec3& cameraLookAt, glm::vec3& cameraUp, float deltaTime) {
	float cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS 
		|| glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
		cameraSpeed = 2.5;
	}
	else {
		cameraSpeed = 0.5;
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		cameraPos += cameraSpeed * cameraLookAt;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		cameraPos -= cameraSpeed * cameraLookAt;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		cameraPos -= glm::normalize(glm::cross(cameraLookAt, cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		cameraPos += glm::normalize(glm::cross(cameraLookAt, cameraUp)) * cameraSpeed;
	}
}
