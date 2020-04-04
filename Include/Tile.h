#pragma once
#pragma once
#include "DrawableModel.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Tile {
public: 
	Tile(GLuint tileVAO, glm::mat4 position, DrawableModel& occupant, glm::vec3 color, const GLuint numVertices);
	
	void draw(GLuint& worldMatrixLocation, GLuint colorLocation);

	void setPosition(glm::mat4 pos) { this->position = pos; };

private:
	GLuint tileVAO;
	DrawableModel& occupant; 
	glm::mat4 position = glm::mat4(1.0f);
	glm::vec3 color = { 1.0f, 1.0f, 1.0f };
	const GLuint& numVertices;
};