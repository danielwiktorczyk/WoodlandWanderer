#include "..\Include\Forest.h"

Forest::Forest(glm::vec3 translatio) {
	std::string cubePath = "../Assets/Models/cube.obj";
	std::string spherePath = "../Assets/Models/sphere.obj";
	std::string treePath = "../Assets/Models/Trees/curvy-pine-tree.obj";

	this->translation = translation;
	this->platform = Model(cubePath, green);

	glm::mat4 platformTransformMatrix = glm::mat4(1.0f);
	platformTransformMatrix = glm::translate(platformTransformMatrix, glm::vec3(0.0f, -3.0f, 0.0f));
	platformTransformMatrix = glm::translate(platformTransformMatrix, translation);
	platformTransformMatrix = glm::scale(platformTransformMatrix, glm::vec3(1000.0f, 3.0f, 1000.0f));
	this->platform.setModelTransformMatrix(platformTransformMatrix);
}

void Forest::draw(GLuint& worldMatrixLocation, GLuint colorLocation) {
	this->platform.draw(worldMatrixLocation, colorLocation);
}


