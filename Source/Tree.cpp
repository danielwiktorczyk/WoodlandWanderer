#include "../Include/Tree.h";

Tree::Tree(GLuint VAO, GLuint numVertices, glm::vec3 color)
	: CollidableModel(VAO, AssetsService::getInstance()->getCube().getVAO(), numVertices, color) {
	/*this->setColliderTransformMatrix(this->getModelTransformMatrix());
	glm::mat4 scalingMatrix = this->getColliderTransformMatrix();
	scalingMatrix = glm::scale(scalingMatrix, glm::vec3(3.0f, 0.1f, 3.0f));*/
	this->setColliderTransformMatrix(this->getModelTransformMatrix());
}

Tree::~Tree() {}