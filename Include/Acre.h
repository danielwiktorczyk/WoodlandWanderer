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
	
	void setPosition(glm::mat4 pos) { this->position = pos; };

private:
	const GLuint& VAO;
	glm::mat4 position = glm::mat4(1.0f);
	glm::vec3 color;
	const GLuint& numVertices;
	std::vector<Tile> tiles; // will be 20 x 20, square?
	bool rendered;
};