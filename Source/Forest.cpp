#include "..\Include\Forest.h"

Forest::Forest(glm::vec3 translatio) {
	this->translation = translation;
	this->platform = Model(cubeAsset, green);

	glm::mat4 platformTransformMatrix = glm::mat4(1.0f);
	platformTransformMatrix = glm::translate(platformTransformMatrix, glm::vec3(0.0f, -3.0f, 0.0f));
	platformTransformMatrix = glm::translate(platformTransformMatrix, translation);
	platformTransformMatrix = glm::scale(platformTransformMatrix, glm::vec3(1000.0f, 3.0f, 1000.0f));
	this->platform.setModelTransformMatrix(platformTransformMatrix);
}

void Forest::draw(GLuint& worldMatrixLocation, GLuint colorLocation) {
	//this->platform.draw(worldMatrixLocation, colorLocation);
}


