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
	Acre(Model& platform);
	
	void draw(GLuint& worldMatrixLocation, GLuint colorLocation);
	
	void setTranslationMatrix(glm::mat4 translationMatrix) { this->translationMatrix = translationMatrix; };

private:
	Model& platform;
	glm::mat4 translationMatrix = glm::mat4(1.0f);
	std::vector<Tile> tiles; // Remains a 10x10, square. TODO Should be an array?
	bool rendered;
};