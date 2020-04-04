#pragma once
#include "DrawableModel.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>

class NonCollidableModel : public DrawableModel {
public:
	NonCollidableModel(GLuint& VAO, const GLuint& numVertices, glm::vec3 color);
};