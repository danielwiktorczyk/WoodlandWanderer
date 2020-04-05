#include "../Include/Acre.h"

Acre::Acre(glm::vec3 translation) {
	std::string cubePath = "../Assets/Models/cube.obj";
	std::string spherePath = "../Assets/Models/sphere.obj";
	std::string treePath = "../Assets/Models/Trees/curvy-pine-tree.obj";
	
	this->translation = translation;
	this->platform = Model(cubePath, orange);
	this->rendered = false;

	glm::mat4 platformTransformMatrix = glm::mat4(1.0f);
	platformTransformMatrix = glm::translate(platformTransformMatrix, glm::vec3(0.0f, -1.5f, 0.0f));
	platformTransformMatrix = glm::translate(platformTransformMatrix, translation);
	platformTransformMatrix = glm::scale(platformTransformMatrix, glm::vec3(100.0f, 1.0f, 100.0f));
	this->platform.setModelTransformMatrix(platformTransformMatrix);
}

void Acre::draw(GLuint& worldMatrixLocation, GLuint colorLocation) {
	this->platform.draw(worldMatrixLocation, colorLocation);
	
	// order all its managed Tiles to draw itself
	// TODO for each Tile tile: tile.draw()
}
