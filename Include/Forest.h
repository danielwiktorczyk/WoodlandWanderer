#pragma once
#include "DrawableModel.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Acre.h"

class Forest {
public: 
	Forest(const GLuint& VAO, const GLuint& numVertices, glm::vec3 color);

	void draw(GLuint& worldMatrixLocationColor, GLuint colorLocation);
	
	GLuint getVAO() { return this->VAO; };
	void setModelTransformMatrix(glm::mat4 modelTransformMatrix) { this->modelTransformMatrix = modelTransformMatrix; };

private:
	const GLuint& VAO;
	const GLuint& numVertices;
	glm::mat4 modelTransformMatrix = glm::mat4(1.0f);
	glm::vec3 color;
	std::vector<Acre> acres; // will be 20 x 20, square?
};