#include "../Include/Tree.h";

/**
* Tree constructor
*/
Tree::Tree(GLuint VAO, GLuint numVertices, glm::vec3 color)
	: CollidableModel(VAO, AssetsService::getInstance()->getCube().getVAO(), numVertices, color) { }

/**
* Tree destructor
*/
Tree::~Tree() {}