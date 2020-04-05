#pragma once
#include "Model.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Tile.h"

#define Width 10

class Acre {
public:
	Acre(glm::vec3 translation);
	void draw(GLuint& worldMatrixLocation, GLuint colorLocation);
	void setTranslationMatrix(glm::vec3 translation) { this->translation = translation; };

private:
	Model platform;
	glm::vec3 translation = glm::vec3(1.0f, 1.0f, 1.0f);
	int width = 10; // must be even!
	std::vector< std::vector<Tile> > tiles; // Remains a 10x10, square. TODO Should be an array?
	bool rendered;
};