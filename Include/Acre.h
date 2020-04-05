#pragma once
#include "DrawableModel.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Tile.h"

class Acre {
public:
	Acre(const GLuint& VAO, const GLuint& numVertices, glm::vec3 color, bool rendered);
	
	void draw(GLuint& worldMatrixLocation, GLuint colorLocation);
	
	void setModelTransformMatrix(glm::mat4 modelTransformMatrix) { this->modelTransformMatrix = modelTransformMatrix; };

private:
	const GLuint& VAO;
	glm::mat4 modelTransformMatrix = glm::mat4(1.0f);
	glm::vec3 color;
	const GLuint& numVertices;
	std::vector<Tile> tiles; // 10x10, square
	bool rendered;
};