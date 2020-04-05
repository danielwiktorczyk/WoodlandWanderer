#include "../Include/Acre.h"

Acre::Acre(Model& platform) : platform(platform) {
	this->translationMatrix = translationMatrix;
	glm::mat4 platformTransformMatrix = glm::mat4(1.0f);
	platformTransformMatrix = glm::scale(platformTransformMatrix, glm::vec3(100.0f, 1.0f, 100.0f));
	platformTransformMatrix = glm::translate(platformTransformMatrix, glm::vec3(0.0f, -0.5f, 0.0f));
	platformTransformMatrix = translationMatrix * platformTransformMatrix;
	this->platform.setModelTransformMatrix(platformTransformMatrix);
	
	this->rendered = false;
}

void Acre::draw(GLuint& worldMatrixLocation, GLuint colorLocation) {
	this->platform.draw(worldMatrixLocation, colorLocation);
	
	// order all its managed Tiles to draw itself
	// TODO for each Tile tile: tile.draw()
}
