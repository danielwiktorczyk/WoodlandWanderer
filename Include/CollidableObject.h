#pragma once
#include "LoadedObject.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>

class CollidableObject : public LoadedObject {
public:
	CollidableObject(GLuint VAO, GLuint numVertices, glm::vec3 color);

};