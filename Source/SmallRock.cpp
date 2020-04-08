#include "..\Include\SmallRock.h"

SmallRock::SmallRock(GLuint VAO, GLuint numVertices, glm::vec3 color)
	: NonCollidableModel(VAO, numVertices, color) { }