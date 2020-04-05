#include "..\Include\Forest.h"

Forest::Forest(Model& platform) : platform(platform) { 
	this->translationMatrix = translationMatrix;
	glm::mat4 platformTransformMatrix = glm::mat4(1.0f);
	platformTransformMatrix = glm::scale(platformTransformMatrix, glm::vec3(1000.0f, 3.0f, 1000.0f));
	platformTransformMatrix = glm::translate(platformTransformMatrix, glm::vec3(0.0f, -3.0f, 0.0f));
	platformTransformMatrix = translationMatrix * platformTransformMatrix;
	this->platform.setModelTransformMatrix(platformTransformMatrix);
}

void Forest::draw(GLuint& worldMatrixLocation, GLuint colorLocation) {
	this->platform.draw(worldMatrixLocation, colorLocation);
}


