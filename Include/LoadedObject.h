#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>

class LoadedObject {
public:
	LoadedObject() = default;
	LoadedObject(GLuint VAO);
	void draw(const GLfloat* objModel,
			  const GLfloat* objColor,
			  GLuint& worldMatrixLocation,
			  GLuint colorLocation,
			  const int& vertices);
	glm::vec3 getPosition();
	void setPosition(glm::vec3 position);

private:
	GLuint VAO;
	glm::vec3 position;
};

