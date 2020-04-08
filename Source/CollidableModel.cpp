#include "..\Include\CollidableModel.h"

CollidableModel::CollidableModel(GLuint VAO, GLuint colliderVAO, GLuint numVertices, glm::vec3 color) 
	: Model(VAO, numVertices, color), colliderVAO(colliderVAO) { }

CollidableModel::~CollidableModel() {}

void CollidableModel::setColliderTransformMatrix(glm::mat4 colliderMatrix) {
	this->colliderTransformMatrix = colliderMatrix;
}

// DEBUG: Overloading Model::draw()
void CollidableModel::draw(const GLuint& worldMatrixLocation, const GLuint& colorLocation) {
	// This is for drawing collidable object
	std::cout << "FOO" << std::endl;
	glDisable(GL_CULL_FACE);

	glBindVertexArray(this->getVAO());
	glBindBuffer(GL_ARRAY_BUFFER, this->getVAO());

	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &(this->getModelTransformMatrix())[0][0]);
	glUniform3fv(colorLocation, 1, value_ptr(this->getColor()));

	glDrawArrays(GL_TRIANGLES, 0, this->getNumVertices());
	glEnable(GL_CULL_FACE);

	// This is for drawing the collider box for Debugging
	glDisable(GL_CULL_FACE);

	glBindVertexArray(this->getColliderVAO());
	glBindBuffer(GL_ARRAY_BUFFER, this->getColliderVAO());

	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &(this->getColliderTransformMatrix())[0][0]);
	glUniform3fv(colorLocation, 1, value_ptr(glm::vec3(1.0f, 0.0f, 0.0f)));

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glEnable(GL_CULL_FACE);
}