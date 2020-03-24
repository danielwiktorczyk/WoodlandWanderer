#include "../Include/Commands.h"
#include <stdlib.h>
#include <time.h>

void Commands::processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}
