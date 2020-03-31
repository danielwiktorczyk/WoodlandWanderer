#include "../Include/Commands.h"

/**
* Checks to see if the user is toggling the textures.
* If the user presses X, the value of texOn is switched
*/
void Commands::areTexturesToggled(GLFWwindow* window, bool& canToggle, bool& texOn) {
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		if (canToggle) {
			texOn = !texOn; // toggle texturing
			canToggle = false;
		}
	}
	else {
		canToggle = true;
	}
}

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
* Pan the camera
*/
void Commands::panCamera(GLFWwindow* window, float& camHorAng, const double& dx, const float& camAngSpeed) {
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		camHorAng -= dx * camAngSpeed;
	}
}

/**
* Tilt the camera
*/
void Commands::tiltCamera(GLFWwindow* window, float& camVertAng, const double& dy, const float& camAngSpeed) {
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
		camVertAng -= dy * camAngSpeed;
	}
}

/**
* Zoom the camera by holding the left mouse button and moving the mouse
*/
void Commands::zoomCamera(GLFWwindow* window, float& currentFOV, const double& dy, glm::mat4& projMatrix, const int& colorShader, const int& textShader, std::function<void(int, glm::mat4)> setProjMatrix) {
	// Camera Zooming
	float cameraZoomSpeed = 0.001f;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		currentFOV += cameraZoomSpeed * dy;

		projMatrix = glm::perspective(currentFOV, 1024.0f / 768.0f, 0.01f, 100.0f);

		setProjMatrix(colorShader, projMatrix);
		setProjMatrix(textShader, projMatrix);
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
