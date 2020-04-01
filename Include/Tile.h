#pragma once
#pragma once
#include "LoadedObject.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Tile {
public: 
	Tile(GLuint tileVAO, GLuint occupantVAO, glm::vec3 position);
	void draw(const GLfloat* value4fv, const GLfloat* value3fv, GLuint& worldMatrixLocationColor, GLuint colorLocation, const int& vertices);

private:
	GLuint tileVAO;
	LoadedObject occupant; 
	glm::vec3 position;
};