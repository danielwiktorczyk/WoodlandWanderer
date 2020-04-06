#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "OBJloader.h"

class Model {
public:
	Model() = default;
	Model(GLuint VAO, GLuint numVertices, glm::vec3 color);

	void draw(const GLuint& worldMatrixLocation, const GLuint& colorLocation);
	
	glm::mat4 getPosition() { return this->modelTransformMatrix; };
	void setModelTransformMatrix(glm::mat4 modelTransformMatrix) { this->modelTransformMatrix = modelTransformMatrix; };
	glm::vec3 getColor() { return this->color; };
	void setColor(glm::vec3 color) { this->color = color; };

private:
	GLuint VAO;
	GLuint numVertices;
	glm::vec3 color;
	glm::mat4 modelTransformMatrix = glm::mat4(1.0f);
};

