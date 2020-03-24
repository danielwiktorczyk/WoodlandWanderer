#pragma once

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

class EnvironmentPlane {
public:
	EnvironmentPlane();
	void draw(Shader& shader);

private:
	GLuint VAO = 0;
	GLuint VBO = 0;
	std::vector<glm::vec3> vertices;
};