//
// COMP 371 Labs Framework
// ASSIGNMENT 1 - GEORGE MAVROEIDIS (40065356)
//
// Created by Nicolas Bergeron on 20/06/2019.
//

#include <iostream>
#include <list>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "OBJloader.h"
#include <Vector>;
#include <String>;

#include <GLFW/glfw3.h> // cross-platform interface for creating a graphical context,
// initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ctime> // for random number


using namespace glm;
using namespace std;

int numOfVertices;

const char* getVertexShaderSource();

const char* getFragmentShaderSource();

const char* getTexturedVertexShaderSource();

const char* getLightVertexShaderSource();

const char* getTexturedFragmentShaderSource();

const char* getLightFragmentShaderSource();

int compileAndLinkShaders(const char* vertexShaderSource, const char* fragmentShaderSource);

int createCubeVertexArrayObject();

int createPyramidVertexArrayObject();

int createPlaneVertexArrayObject();

GLuint setupModelVBO(std::string path, int& vertexCount);

int createLightVertexArrayObject();

GLuint loadTexture(const char* filename);

bool initContext();

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

GLFWwindow * window = NULL;

const float windowWidth = 768;
const float windowLength = 1024;
bool firstMouse = true;
float fov = 70.0f;

GLboolean CheckCollision(vec3 &one, vec3 &two);

//MODEL-VIEW METHODS FOR SHADER
void setProjectionMatrix(int shaderProgram, mat4 projectionMatrix)
{
	glUseProgram(shaderProgram);
	GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
}
void setViewMatrix(int shaderProgram, mat4 viewMatrix)
{
	glUseProgram(shaderProgram);
	GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
}
void setWorldMatrix(int shaderProgram, mat4 worldMatrix)
{
	glUseProgram(shaderProgram);
	GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
}
void setOrientationMatrix(int shaderProgram, mat4 orientationMatrix)
{
	glUseProgram(shaderProgram);
	GLuint orientationMatrixLocation = glGetUniformLocation(shaderProgram, "orientationMatrix");
	glUniformMatrix4fv(orientationMatrixLocation, 1, GL_FALSE, &orientationMatrix[0][0]);
}

bool keyA = true;
bool keyD = true;
bool keyS = true;
bool keyW = true;

void CheckA( bool checkA, bool isColliding) {
	if (checkA && isColliding) {
		keyA = false;
	}
	else {
		keyA = true;
	}
}
void CheckD(bool checkD, bool isColliding) {
	if (checkD && isColliding) {
		keyD = false;
	}
	else {
		keyD = true;
	}
}
void CheckW(bool checkW, bool isColliding) {
	if (checkW && isColliding) {
		keyW = false;
	}
	else {
		keyW = true;
	}
}
void CheckS(bool checkS, bool isColliding) {
	if (checkS && isColliding) {
		keyS = false;
	}
	else {
		keyS = true;
	}
}

GLboolean CheckCollision(vec3 &one, vec3 &two) // AABB - AABB collision
{
	// Collision x-axis?
	bool collisionX = one.x + 2 >= two.x &&
		two.x + 2 >= one.x;
	// Collision y-axis?
	bool collisionZ = one.z + 2 >= two.z &&
		two.z + 2 >= one.z;
	// Collision only if on both axes
	return collisionX && collisionZ;
}

bool CheckCollisionA(float x1, float x2) {
	return x1 + 2 >= x2;
}

bool CheckCollisionD(float x1, float x2) {
	return x2 + 2 <= x1;
}

bool CheckCollisionW(float z1, float z2) {
	return z1 + 2 >= z2;
}

bool CheckCollisionS(float z1, float z2) {
	return z2 + 2 <= z1;
}

int main(int argc, char*argv[])
{
	if (!initContext()) return -1;

	// Load Textures
	GLuint snowTextureID = loadTexture("C:/Users/G/Documents/GitHub/WoodlandWanderer/Source/TestCollisions/now.jpg");
	GLuint snow2TextureID = loadTexture("C:/Users/G/Documents/GitHub/WoodlandWanderer/Source/TestCollisions/snow2.jpg");
	GLuint redTextureID = loadTexture("C:/Users/G/Documents/GitHub/WoodlandWanderer/Source/TestCollisions/red.jpg");
	GLuint greenTextureID = loadTexture("C:/Users/G/Documents/GitHub/WoodlandWanderer/Source/TestCollisions/green.jpg");
	GLuint blueTextureID = loadTexture("C:C:/Users/G/Documents/GitHub/WoodlandWanderer/Source/TestCollisions/blue.jpg");

	// Background Color
	glClearColor(0.2f, 0.2f, 0.5f, 1.0f);

	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	// Compile and link shaders here ...

	int shaderProgram = compileAndLinkShaders(getVertexShaderSource(), getFragmentShaderSource());
	int textureProgram = compileAndLinkShaders(getTexturedVertexShaderSource(), getTexturedFragmentShaderSource());
	int lightProgram = compileAndLinkShaders(getLightVertexShaderSource(), getLightFragmentShaderSource());


	// INITIAL Camera parameters for view transform
	vec3 cameraPosition(0.0f, 5.0f, 20.0f);
	vec3 cameraLookAt(0.0f, 0.0f, 0.0f);
	vec3 cameraUp(0.0f, 1.0f, 0.0f);
	// Other camera parameters
	float cameraSpeed = 1.0f;
	float cameraFastSpeed = 7 * cameraSpeed;
	float cameraHorizontalAngle = 90.0f;
	float cameraVerticalAngle = 0.0f;


	// SET PERSPECTIVE VIEW
	mat4 projectionMatrix = glm::perspective(glm::radians(fov),            // field of view in degrees
		800.0f / 600.0f,  // aspect ratio
		0.01f, 100.0f);   // near and far (near > 0)

	// Set initial view matrix
	mat4 viewMatrix = lookAt(cameraPosition,  // eye
		cameraLookAt,  // center
		cameraUp); // up

	setViewMatrix(shaderProgram, viewMatrix);
	setViewMatrix(textureProgram, viewMatrix);
	setViewMatrix(lightProgram, viewMatrix);

	setProjectionMatrix(shaderProgram, projectionMatrix);
	setProjectionMatrix(textureProgram, projectionMatrix);
	setProjectionMatrix(lightProgram, projectionMatrix);


	// Define and upload geometry to the GPU here
	int cubeVAO = setupModelVBO("C:/Users/G/Documents/GitHub/opengl-practise/Lab_Framework/Source/Code/cube.obj", numOfVertices);
	int pyramidVAO = setupModelVBO("C:/Users/G/Documents/GitHub/opengl-practise/Lab_Framework/Source/Code/pyramid.obj", numOfVertices);
	int handVAO = setupModelVBO("C:/Users/G/Documents/GitHub/opengl-practise/Lab_Framework/Source/Code/hand.obj", numOfVertices);
	int hatVAO = setupModelVBO("C:/Users/G/Documents/GitHub/opengl-practise/Lab_Framework/Source/Code/cap.obj", numOfVertices);
	// sphere2.obj may take a lot of GPU power because it has over 200k vertices
	// replace it with sphere.obj for faster rendermode
	int scarfVAO = setupModelVBO("C:/Users/G/Documents/GitHub/opengl-practise/Lab_Framework/Source/Code/scarf.obj", numOfVertices);
	int sphereVAO = setupModelVBO("C:/Users/G/Documents/GitHub/opengl-practise/Lab_Framework/Source/Code/sphere2.obj", numOfVertices);
	int lightVAO = createLightVertexArrayObject();

	int colliderVAO = createLightVertexArrayObject();

	// For frame time
	float lastFrameTime = glfwGetTime();
	int lastMouseLeftState = GLFW_RELEASE;
	int lastMouseRightState = GLFW_RELEASE;
	int lastMouseMiddleState = GLFW_RELEASE;
	double lastMousePosX, lastMousePosY;
	glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);

	// Other OpenGL states to set once before the Game Loop
	// Enable Backface culling
	//glEnable(GL_CULL_FACE);

	srand(time(NULL));
	GLfloat random1 = 0.0f;
	GLfloat random2 = 0.0f;

	GLuint colorLocation = glGetUniformLocation(shaderProgram, "objectColor"); // Object Color
	GLuint lightLocation = glGetUniformLocation(textureProgram, "lightColor"); // Object Color
	GLuint lightLocation2 = glGetUniformLocation(shaderProgram, "lightColor"); // Object Color

	mat4 olafWorldMatrix; // olaf matrix
	mat4 gizmoWorldMatrix; // gizmo matrix
	mat4 gridWorldMatrix; // grid matrix
	mat4 lightWorldMatrix; // light source matrix
	mat4 model = mat4(1.0f); // identity matrix
	vec3 olafPosition(0.0f, 0.0f, 0.0f); // initial olag position

	mat4 orientationMatrix = mat4(1.0f); // initialize orientation matrix
	vec2 currentOrientation(0.0f, 0.0f); // current orientation of matrix

	mat4 groupMatrix = mat4(1.0f); // matrix for group 
	mat4 bodyMatrix = mat4(1.0f); // rotation matrix for Olaf
	mat4 armMatrix = mat4(1.0f); // rotation of arms of Olaf
	mat4 scaleMatrix = mat4(1.0f); // scale matrix for Olaf objects
	vec3 currentRotation(0.0f, 0.0f, 0.0f); // current rotation for rotation matrix
	vec3 currentRotationArms(0.0f, 0.0f, 0.0f); // arm movement
	vec3 currentScale(1.0f, 1.0f, 1.0f); // currentScale applied to scale Matrix
	
	int i = 0;
	// Entering Game Loop
	while (!glfwWindowShouldClose(window))
	{
		
		// Add the GL_DEPTH_BUFFER_BIT to glClear – TODO 1
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Frame time calculation
		float dt = glfwGetTime() - lastFrameTime;
		lastFrameTime += dt;

		// generate random coordinates for olaf
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			random1 = (rand() % 50) - 25;
			random2 = (rand() % 50) - 25;
		}

		

		// Draw geometry for cube

		// set orientation matrix
		orientationMatrix = rotate(rotate(mat4(1.0f), currentOrientation.x, vec3(1.0f, 0.0f, 0.0f)), currentOrientation.y, vec3(0.0f, 1.0f, 0.0f));
		setOrientationMatrix(shaderProgram, orientationMatrix);
		setOrientationMatrix(textureProgram, orientationMatrix);
		setOrientationMatrix(lightProgram, orientationMatrix);


		bodyMatrix = rotate(model, currentRotation.y, vec3(0.0f, 1.0f, 0.0f));
		groupMatrix = translate(model, vec3(random1, 0.0f, random2)) *  translate(model, olafPosition) * scale(model, currentScale);
		

		// Light Source
		glBindVertexArray(lightVAO);
		glBindTexture(GL_TEXTURE_2D, snow2TextureID);
		gridWorldMatrix = translate(model, vec3(0.0f, 10.0f, 0.0f)) * scale(model, vec3(2.0f, 2.0f, 2.0f));
		setWorldMatrix(lightProgram, gridWorldMatrix);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// GIZMO
		// X-axis
		glBindVertexArray(cubeVAO);
		glBindTexture(GL_TEXTURE_2D, redTextureID);
		gizmoWorldMatrix = translate(model, vec3(2.5f, 0.0f, 0.0f)) * scale(model, vec3(5.0f, 0.1f, 0.1f));
		setWorldMatrix(textureProgram, gizmoWorldMatrix);
		glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.0, 0.0, 0.0)));
		glUniform3fv(lightLocation, 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// Y-axis
		glBindTexture(GL_TEXTURE_2D, greenTextureID);
		gizmoWorldMatrix = translate(model, vec3(0.0f, 2.5f, 0.0f)) * scale(model, vec3(0.1f, 5.0f, 0.1f));
		setWorldMatrix(textureProgram, gizmoWorldMatrix);
		glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(0.0, 1.0, 0.0)));
		glUniform3fv(lightLocation, 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// Z-axis
		glBindTexture(GL_TEXTURE_2D, blueTextureID);
		gizmoWorldMatrix = translate(model, vec3(0.0f, 0.0f, 2.5f)) * scale(model, vec3(0.1f, 0.1f, 5.0f));
		setWorldMatrix(textureProgram, gizmoWorldMatrix);
		glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(0.0, 0.0, 1.0)));
		glUniform3fv(lightLocation, 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
		glDrawArrays(GL_TRIANGLES, 0, 36);

	
		// collider1
		glBindVertexArray(cubeVAO);
		glBindTexture(GL_TEXTURE_2D, redTextureID);
		mat4 moverMatrix = groupMatrix * translate(model, vec3(0.0f, 1.0f, 0.0f)) * scale(model, vec3(2.0f, 2.0f, 2.0f));
		setWorldMatrix(textureProgram, moverMatrix);
		glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(0.0, 0.0, 1.0)));
		glUniform3fv(lightLocation, 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//collider 2
		glBindVertexArray(cubeVAO);
		glBindTexture(GL_TEXTURE_2D, blueTextureID);
		vec3 colliderPos = vec3(5.0f, 1.0f, 5.0f);
		mat4 moverMatrix2 = translate(model, colliderPos) * scale(model, vec3(2.0f, 2.0f, 2.0f));
		setWorldMatrix(textureProgram, moverMatrix2);
		glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(0.0, 0.0, 1.0)));
		glUniform3fv(lightLocation, 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Plane Textured
		glBindVertexArray(cubeVAO);
		glBindTexture(GL_TEXTURE_2D, snowTextureID);
		gridWorldMatrix = rotate(model, radians(-90.0f), vec3(1.0f, 0.0f, 0.0f)) * scale(model, vec3(100.0f, 100.0f, 0.01f));
		setWorldMatrix(textureProgram, gridWorldMatrix);
		glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		glUniform3fv(lightLocation, 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
		glDrawArrays(GL_TRIANGLES, 0, numOfVertices);

		//DEBUGGER FOR COLLISION
		/*
		if (CheckCollision(olafPosition, colliderPos)) {
			cout << "COLLISION DETECTED"<< " " << i << endl;
			i++;
		}*/

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		double mousePosX, mousePosY;
		glfwGetCursorPos(window, &mousePosX, &mousePosY);
		

		// CAMERA TILT AND PAN
		double dx = 0;
		double dy = 0;
		if (lastMouseRightState == GLFW_RELEASE && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
			dx = mousePosX - lastMousePosX;
		}
		if (lastMouseMiddleState == GLFW_RELEASE && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			dy = mousePosY - lastMousePosY;
		}
		

		lastMousePosX = mousePosX;
		lastMousePosY = mousePosY;

		// Convert to spherical coordinates
		const float cameraAngularSpeed = 30.0f;
		cameraHorizontalAngle -= dx * cameraAngularSpeed * dt;
		cameraVerticalAngle -= dy * cameraAngularSpeed * dt;

		// Clamp vertical angle to [-85, 85] degrees
		cameraVerticalAngle = max(-85.0f, min(85.0f, cameraVerticalAngle));
		if (cameraHorizontalAngle > 360)
		{
			cameraHorizontalAngle -= 360;
		}
		else if (cameraHorizontalAngle < -360)
		{
			cameraHorizontalAngle += 360;
		}

		float theta = radians(cameraHorizontalAngle);
		float phi = radians(cameraVerticalAngle);

		cameraLookAt = vec3(cosf(phi)*cosf(theta), sinf(phi), -cosf(phi)*sinf(theta));
		vec3 cameraSideVector = glm::cross(cameraLookAt, vec3(0.0f, 1.0f, 0.0f));

		glm::normalize(cameraSideVector);


		// TRANSLATION INPUT WITH CHECKED COLLISIONS
		// !CheckCollision(olafPosition, vec3(5.0f, 1.0f, 20.0f))
		//CheckCollisionX(olafPosition.x, colliderPos.x);
		bool checker = CheckCollision(olafPosition, vec3(5.0f, 1.0f, 20.0f));
		bool checkA = CheckCollisionA(olafPosition.x, colliderPos.x);
		bool checkD = CheckCollisionA(olafPosition.x, colliderPos.x);
		bool checkW = CheckCollisionA(olafPosition.z, colliderPos.z);
		bool checkS = CheckCollisionA(olafPosition.z, colliderPos.z);
		//CheckCollisionX(olafPosition.x, colliderPos.x);
		CheckA(checkA, checker);
		CheckD(checkD, checker);
		CheckW(checkW, checker);
		CheckS(checkS, checker);
		cout << "CHECKER: " << checker << endl;
		cout << "x1: " << olafPosition.x << ", x2: " << colliderPos.x << endl;
		cout << "keyA: " << keyA << ", keyD: " << keyD << endl;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && keyA) // move camera to the left
		{
			olafPosition.x += 5 * dt;
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && keyD) // move camera to the right
		{
			olafPosition.x -= 5 * dt;
		}
		//CheckCollisionZ(olafPosition.z, colliderPos.z);
		cout << "z1: " << olafPosition.z << ", z2: " << colliderPos.z << endl;
		cout << "keyS: " << keyS << ", keyW: " << keyW << endl;
		cout << endl;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && keyS) // move camera up
		{
			olafPosition.z -= 5 * dt;
		}

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && keyW) // move camera down
		{
			olafPosition.z += 5 * dt;
		}



		// ROTATION INPUT
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) // move camera to the left
		{
			currentRotation.y += radians(2.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) // move camera to the left
		{
			currentRotation.y -= radians(2.0f);
		}

		// SCALE INPUT
		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) // move camera to the left
		{
			currentScale.x += 0.1f;
			currentScale.y += 0.1f;
			currentScale.z += 0.1f;
		}
		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) // move camera to the left
		{
			currentScale.x -= 0.1f;
			currentScale.y -= 0.1f;
			currentScale.z -= 0.1f;
		}

		// ORIENTATION INPUT (CHANGE COORDINATES)
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			currentOrientation.x += radians(5.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			currentOrientation.x -= radians(5.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			currentOrientation.y += radians(5.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			currentOrientation.y -= radians(5.0f);
		}
		// reset orientation
		if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS) {
			currentOrientation.y = 0;
			currentOrientation.x = 0;
		}



		// RENDER TYPES
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) 
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		}
		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);
		}
		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		// PROJECTION MATRIX
		projectionMatrix = glm::perspective(glm::radians(fov),            // field of view in degrees
			800.0f / 600.0f,  // aspect ratio
			0.01f, 100.0f);   // near and far (near > 0)
		setProjectionMatrix(textureProgram, projectionMatrix);
		setProjectionMatrix(lightProgram, projectionMatrix);

		// VIEW MATRIX
		viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);
		setViewMatrix(textureProgram, viewMatrix);
		setViewMatrix(lightProgram, viewMatrix);

		// End Frame
		glfwSwapBuffers(window);
		glfwPollEvents();

	}


	// Shutdown GLFW
	glfwTerminate();

	return 0;
}

const char* getVertexShaderSource()
{
	// For now, you use a string for your shader code, in the assignment, shaders will be stored in .glsl files
	return
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;"
		"layout (location = 1) in vec3 aColor;"
		""
		"uniform mat4 worldMatrix;"
		"uniform mat4 viewMatrix = mat4(1.0);"  // default value for view matrix (identity)
		"uniform mat4 projectionMatrix = mat4(1.0);"
		""
		"out vec3 vertexColor;"
		"void main()"
		"{"
		"   vertexColor = aColor;"
		"   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;"
		"   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
		"}";
}

const char* getFragmentShaderSource()
{
	return
		"#version 330 core\n"
		"in vec3 vertexColor;"
		"out vec4 FragColor;"
		"void main()"
		"{"
		"   FragColor = vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f);"
		"}";
}

const char* getTexturedVertexShaderSource()
{
	// For now, you use a string for your shader code, in the assignment, shaders will be stored in .glsl files
	return
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aNormal;\n"
		"layout (location = 2) in vec2 aTexCoord;\n"
		""
		"uniform mat4 worldMatrix = mat4(1.0f);\n"
		"uniform mat4 orientationMatrix = mat4(1.0);\n"
		"uniform mat4 viewMatrix = mat4(1.0);\n"
		"uniform mat4 projectionMatrix = mat4(1.0);\n"
		""
		"out vec2 TexCoord;\n"
		"out vec3 Normal;\n"
		"out vec3 FragPos;\n"
		""
		"void main()\n"
		"{\n"
		"	FragPos = vec3(worldMatrix * vec4(aPos, 1.0));\n"
		"	Normal = mat3(transpose(inverse(worldMatrix))) * aNormal;\n"
		""
		"   mat4 modelViewProjection = projectionMatrix * viewMatrix * orientationMatrix * worldMatrix;\n"
		"   gl_Position = modelViewProjection * vec4(aPos, 1.0);\n"
		""
		"	TexCoord = aTexCoord;\n"
		"}\n";
}

const char* getTexturedFragmentShaderSource()
{
	return
		"#version 330 core\n"
		"in vec2 TexCoord;\n"
		"in vec3 Normal;\n"
		"in vec3 FragPos;\n"
		""
		"uniform sampler2D textureSampler;\n"
		"uniform vec3 lightPos = vec3(0.0f, 10.0f, 0.0f);\n"
		"uniform vec3 viewPos;\n"
		""
		"out vec4 FragColor;\n"
		""
		"void main()\n"
		"{\n"
		"	vec4 textureColor = texture(textureSampler, TexCoord);\n"
		""
		"	float ambientStrength = 0.5;\n"
		"	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);\n"
		"	vec3 ambient = ambientStrength * lightColor;\n"
		""
		"	vec3 norm = normalize(Normal);\n"
		"	vec3 lightDir = normalize(lightPos - FragPos);\n"
		"	float diff = max(dot(norm, lightDir), 0.0);\n"
		"	vec3 diffuse = diff * lightColor;\n"
		""
		"	float specularStrength = 0.3;\n"
		"	vec3 viewDir = normalize(viewPos - FragPos);\n"
		"	vec3 reflectDir = reflect(-lightDir, norm);\n"
		"	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);\n"
		"	vec3 specular = specularStrength * spec * lightColor;\n"
		""
		"	vec3 result = (ambient + diffuse + specular) * vec3(textureColor.r, textureColor.g, textureColor.b);\n"
		""
		"   FragColor = vec4(result, 1.0);\n"
		"}\n";
}

const char* getLightVertexShaderSource() {
	// For now, you use a string for your shader code, in the assignment, shaders will be stored in .glsl files
	return
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		""
		"uniform mat4 worldMatrix;\n"
		"uniform mat4 orientationMatrix = mat4(1.0);\n"
		"uniform mat4 viewMatrix = mat4(1.0)\n;"
		"uniform mat4 projectionMatrix = mat4(1.0);\n"
		""
		"void main()\n"
		"{\n"
		"   mat4 modelViewProjection = projectionMatrix * viewMatrix * orientationMatrix * worldMatrix;\n"
		"   gl_Position = modelViewProjection * vec4(aPos, 1.0);\n"
		"}\n";
}

const char* getLightFragmentShaderSource() {
	return
		"#version 330 core\n"
		"out vec4 FragColor\n;"
		""
		"void main()\n"
		"{\n"
		"	FragColor = vec4(1.0);\n"
		"}\n";
}

int compileAndLinkShaders(const char* vertexShaderSource, const char* fragmentShaderSource)
{
	// compile and link shader program
	// return shader program id
	// ------------------------------------

	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// link shaders
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

int createCubeVertexArrayObject()
{
	struct TexturedColoredVertex
	{
		TexturedColoredVertex(vec3 _position, vec3 _normal, vec2 _uv)
			: position(_position), normal(_normal), uv(_uv) {}

		vec3 position;
		vec3 normal;
		vec2 uv;
	};

	// Textured Cube model
	const TexturedColoredVertex texturedCubeVertexArray[] = {  // position,                            color
		TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f,  0.0f, -1.0f), vec2(0.0f, 0.0f)), //left - red
		TexturedColoredVertex(vec3(-0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 1.0f)),
		TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f)),

		TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)),
		TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f)),
		TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 0.0f)),

		TexturedColoredVertex(vec3(0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f)), // far - blue
		TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)),
		TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f)),

		TexturedColoredVertex(vec3(0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f)),
		TexturedColoredVertex(vec3(0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f)),
		TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)),

		TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)), // bottom - turquoise
		TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),
		TexturedColoredVertex(vec3(0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f)),

		TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)),
		TexturedColoredVertex(vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f)),
		TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),

		TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f)), // near - green
		TexturedColoredVertex(vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f)),
		TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f)),

		TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f)),
		TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f)),
		TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f)),

		TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f)), // right - purple
		TexturedColoredVertex(vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)),
		TexturedColoredVertex(vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f)),

		TexturedColoredVertex(vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)),
		TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f)),
		TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f)),

		TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f)), // top - yellow
		TexturedColoredVertex(vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f)),
		TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f)),

		TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f)),
		TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f)),
		TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f))
	};





	// Create a vertex array
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);


	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texturedCubeVertexArray), texturedCubeVertexArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		sizeof(TexturedColoredVertex), // stride - each vertex contain 2 vec3 (position, color)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(TexturedColoredVertex),
		(void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
	);
	glEnableVertexAttribArray(1);

	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedColoredVertex), (void*)(2 * sizeof(vec3)));
	glEnableVertexAttribArray(2);

	//glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	//glBindVertexArray(0);

	return vertexArrayObject;
}

int createPyramidVertexArrayObject()
{
	// Cube model

	vec3 vertexArray[] = {  // position,                            color
		vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f),
		vec3(-1.0f, -1.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f),
		vec3(1.0f, -1.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f),
		
		vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f),
		vec3(1.0f, -1.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f),
		vec3(1.0f, -1.0f, -1.0f), vec3(0.0f, 0.0f, 0.0f),

		vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f),
		vec3(1.0f, -1.0f, -1.0f), vec3(0.0f, 0.0f, 0.0f),
		vec3(-1.0f, -1.0f, -1.0f), vec3(0.0f, 0.0f, 0.0f),
		
		vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f),
		vec3(-1.0f, -1.0f, -1.0f), vec3(0.0f, 0.0f, 0.0f),
		vec3(-1.0f, -1.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f),
		
};
	// grid model
	GLuint indexArray[] = {
		0, 1, 2,
		0, 2, 3
	};





	// Create a vertex array
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);


	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);
	/*
	GLuint elsementBufferObject;
	glGenBuffers(1, &elementBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);
	*/

	glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		2 * sizeof(vec3), // stride - each vertex contain 2 vec3 (position, color)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		2 * sizeof(vec3),
		(void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
	);
	glEnableVertexAttribArray(1);

	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)(6 * sizeof(vec3)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBindVertexArray(0);

	return vertexArrayObject;
}

GLuint setupModelVBO(std::string path, int& vertexCount) {
	vector<glm::vec3> vertices;
	vector<glm::vec3> normals;
	vector<glm::vec2> UVs;

	//read the vertex data from the model's OBJ file
	loadOBJ(path.c_str(), vertices, normals, UVs);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Vertex VBO setup
	GLuint vertices_VBO;
	glGenBuffers(1, &vertices_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertices_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Normals VBO setup
	GLuint normals_VBO;
	glGenBuffers(1, &normals_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, normals_VBO);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);

	//UVs VBO setup
	GLuint uvs_VBO;
	glGenBuffers(1, &uvs_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, uvs_VBO);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	vertexCount = vertices.size();
	return VAO;
}

int createPlaneVertexArrayObject() {
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return VAO;
}

int createLightVertexArrayObject() {
	//cube model
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};

	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	// we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	return lightVAO;
}

GLuint loadTexture(const char *filename)
{
	// Step1 Create and bind textures
	GLuint textureId = 0;
	glGenTextures(1, &textureId);
	assert(textureId != 0);


	glBindTexture(GL_TEXTURE_2D, textureId);

	// Step2 Set filter parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Step3 Load Textures with dimension data
	int width, height, nrChannels;
	unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (!data)
	{
		std::cerr << "Error::Texture could not load texture file:" << filename << std::endl;
		return 0;
	}

	// Step4 Upload the texture to the GPU
	GLenum format = 0;
	if (nrChannels == 1)
		format = GL_RED;
	else if (nrChannels == 3)
		format = GL_RGB;
	else if (nrChannels == 4)
		format = GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height,
		0, format, GL_UNSIGNED_BYTE, data);

	// Step5 Free resources
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
	return textureId;
}

bool initContext() {     // Initialize GLFW and OpenGL version
	glfwInit();

#if defined(PLATFORM_OSX)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
	// On windows, we set OpenGL version to 2.1, to support more hardware
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif

	// Create Window and rendering context using GLFW, resolution is 1024, 768
	window = glfwCreateWindow(windowLength, windowWidth, "Comp371 - Assignment 1", NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	// Scroll Callback for field of view
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to create GLEW" << std::endl;
		glfwTerminate();
		return false;
	}
	return true;
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 100.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 100.0f)
		fov = 100.0f;
}
