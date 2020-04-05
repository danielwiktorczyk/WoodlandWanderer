#pragma once
#pragma once
#include "Model.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Tile {
public: 
	Tile(Model& platform, Model& occupant, glm::mat4 modelTransformMatrix);
	
	void draw(GLuint& worldMatrixLocation, GLuint colorLocation);

	void setModelTransformMatrix(glm::mat4 modelTransformMatrix) { this->translationMatrix = modelTransformMatrix; };

private:
	Model& platform;
	Model& occupant; 
	glm::mat4 translationMatrix = glm::mat4(1.0f);
};