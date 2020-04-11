#include "../Include/Tree.h";

/**
* Tree constructor
*/
Tree::Tree(GLuint VAO, GLuint numVertices, glm::vec3 color)
	: CollidableModel(VAO, numVertices, color) { }

/**
* Tree destructor
*/
Tree::~Tree() {}