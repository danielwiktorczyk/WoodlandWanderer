#pragma once

#include "Snowman.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>

enum CameraMode { freeCamera, thirdPersonCamera, firstPersonCamera };

class Camera {
public: 
	Camera();
	void processFreeCamera(GLFWwindow* window, glm::vec3& cameraPos, glm::vec3& cameraLookAt, glm::vec3& cameraUp);
	void processThirdPersonCamera(GLFWwindow* window, glm::vec3& cameraPos, glm::vec3& cameraLookAt, glm::vec3& cameraUp, Snowman& snowman);
	void processFirstPersonCamera(GLFWwindow* window, glm::vec3& cameraPos, glm::vec3& cameraLookAt, glm::vec3& cameraUp, Snowman& snowman);
	void process(GLFWwindow* window, glm::vec3& cameraPos, glm::vec3& cameraLookAt, glm::vec3& cameraUp, Snowman& snowman);
	void mouseCallback(GLFWwindow* window, double xPos, double yPos);
	void setProjectionMatrix(const int& shaderProgram);
	void setViewMatrix(const int& shaderProgram);

private: 
	CameraMode cameraMode = freeCamera;

	// Camera parameters for view transform
	glm::vec3 cameraPosition = glm::vec3(3.0f, 5.0f, 25.0f);
	glm::vec3 cameraLookAt = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 projectionMatrix = glm::perspective(fov, (HD ? WindowWidthHD : WindowWidth) / (HD ? WindowHeightHD : WindowHeight), 0.01f, 100.0f);
	glm::mat4 viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);

	// Other camera parameters
	bool firstMouse = true;
	float yaw = -90.0f;
	float pitch = 0.0f;
	float lastX = (HD ? WindowWidthHD : WindowWidth) / 2.0;
	float lastY = 768 / 2.0;
	float fov = 45.0f;
	float lastFrame = 0.0f;
};


