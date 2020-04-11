#include "../Include/BigRock.h";

/**
* BigRock constructor
*/
BigRock::BigRock(GLuint VAO, GLuint numVertices, glm::vec3 color)
	: CollidableModel(VAO, numVertices, color) { }

/**
* BigRock destructor
*/
BigRock::~BigRock() {}