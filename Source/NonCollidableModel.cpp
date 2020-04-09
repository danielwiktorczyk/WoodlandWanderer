#include "..\Include\NonCollidableModel.h"

/**
* constructor
*/
NonCollidableModel::NonCollidableModel(GLuint VAO, GLuint numVertices, glm::vec3 color)
	: Model(VAO, numVertices, color) { }

/**
* Destructor
*/
NonCollidableModel::~NonCollidableModel() { }
