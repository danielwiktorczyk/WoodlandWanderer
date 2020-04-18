#include "../Include/Camera.h";

Camera::Camera() {
	this->cameraMode = freeCamera;
};

/* 
For roaming around without being hitched to the snowman. Excellent to debug and explore the scene! 
*/
void Camera::processFreeCamera(GLFWwindow* window, glm::vec3& cameraPos, glm::vec3& cameraLookAt, glm::vec3& cameraUp)
{
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

/* 
Follow the snowman at a distance behind #socialDistancing
*/
void Camera::processThirdPersonCamera(GLFWwindow* window, glm::vec3& cameraPos, glm::vec3& cameraLookAt, glm::vec3& cameraUp, Snowman& snowman)
{
	glm::vec2 snowmanLookat = glm::vec2(sin(snowman.rotation), cos(snowman.rotation));
	cameraLookAt = glm::vec3(snowmanLookat.x, cameraLookAt.y, snowmanLookat.y);

	cameraPos = glm::vec3(snowman.origin.x - 16 * snowmanLookat.x, 5.0f + 5.0f, snowman.origin.z - 16 * snowmanLookat.y);
}

void Camera::processFirstPersonCamera(GLFWwindow* window, glm::vec3& cameraPos, glm::vec3& cameraLookAt, glm::vec3& cameraUp, Snowman& snowman)
{
	glm::vec2 snowmanLookat = glm::vec2(sin(snowman.rotation), cos(snowman.rotation));
	cameraLookAt = glm::vec3(snowmanLookat.x, cameraLookAt.y, snowmanLookat.y);

	cameraPos = glm::vec3(snowman.origin.x, 5.0f, snowman.origin.z);

	// WIP for getting look-around capability while in first / third person modes
	//// Get the two vectors
	//glm::vec2 snowmanLookat = glm::vec2(-sin(snowman.rotation), -cos(snowman.rotation));
	//glm::vec2 desiredLookat = glm::vec2(cameraLookAt.x, cameraLookAt.z);

	//// Center at origin
	//snowmanLookat = snowmanLookat - glm::vec2(snowman.origin.x, snowman.origin.z);
	//desiredLookat = desiredLookat - glm::vec2(snowman.origin.x, snowman.origin.z);
	//
	//// Normalize them
	//snowmanLookat = glm::normalize(snowmanLookat);
	//desiredLookat = glm::normalize(desiredLookat);

	//// Dot product
	//float dotProduct = glm::dot(snowmanLookat, desiredLookat);
	//float angle = acos(dotProduct);

	//// Depending on the angle, constrain the lookat view!
	//if (angle < M_PI / 3 || angle > 5 * M_PI / 3) {
	//	return;
	//}
	//else if (angle < M_PI) { // clamp left side
	//	cameraLookAt = glm::vec3(
	//		snowmanLookat.x * cos(M_PI / 3) + snowmanLookat.y * sin(M_PI / 3),
	//		cameraLookAt.y,
	//		snowmanLookat.x * sin(M_PI / 3) - snowmanLookat.y * cos(M_PI / 3));
	//}
	//else {
	//	cameraLookAt = glm::vec3(
	//		snowmanLookat.x * cos(5 * M_PI / 3) + snowmanLookat.y * sin(5 * M_PI / 3),
	//		cameraLookAt.y,
	//		snowmanLookat.x * sin(5 * M_PI / 3) - snowmanLookat.y * cos(5 * M_PI / 3));
	//}

	//angle = angle;
}

void Camera::process(GLFWwindow* window, glm::vec3& cameraPos, glm::vec3& cameraLookAt, glm::vec3& cameraUp, Snowman& snowman)
{
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		this->cameraMode = firstPersonCamera;
		snowman.disableRendering();
	}
	else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		this->cameraMode = thirdPersonCamera;
		snowman.enableRendering();
	}
	else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
		this->cameraMode = freeCamera;
		snowman.enableRendering();
	}

	switch(this->cameraMode) {
	case firstPersonCamera:
		processFirstPersonCamera(window, cameraPos, cameraLookAt, cameraUp, snowman);
		snowman.disableRendering();
		break;
	case thirdPersonCamera:
		processThirdPersonCamera(window, cameraPos, cameraLookAt, cameraUp, snowman);
		break;
	case freeCamera:
		processFreeCamera(window, cameraPos, cameraLookAt, cameraUp);
		break;
	}
	
}
