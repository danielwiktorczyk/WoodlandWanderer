#include "..\Include\Grass.h"

Grass::Grass(GLuint VAO, GLuint numVertices, glm::vec3 color)
	: NonCollidableModel(VAO, numVertices, color) { }

/**
* Destructor
*/
Grass::~Grass() { }
