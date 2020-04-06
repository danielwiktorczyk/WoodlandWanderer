#include "../Include/Acre.h"

Acre::Acre(glm::vec3 translation) {
	this->translation = translation;
	this->rendered = false;

	this->platform = Model(cubeAsset, darkPurple);
	glm::mat4 platformTransformMatrix = glm::mat4(1.0f);
	platformTransformMatrix = glm::translate(platformTransformMatrix, glm::vec3(0.0f, -0.5f, 0.0f));
	platformTransformMatrix = glm::translate(platformTransformMatrix, translation);
	platformTransformMatrix = glm::scale(platformTransformMatrix, glm::vec3(Width * 10.0f, 1.0f, Width * 10.0f));
	this->platform.setModelTransformMatrix(platformTransformMatrix);

	this->tiles.resize(Width);
	for (int i = 0; i < Width; ++i)
		this->tiles[i].resize(Width);
	int upperBound = (Width - 1) / 2 ;
	int lowerBound = -upperBound;
	for (int i = 0; i < Width; i++) 
		for (int j = 0; j < Width; j++) 
			tiles[i][j] = Tile(translation + glm::vec3(10.0f * (lowerBound + i), 0.0f, 10.0f * (lowerBound + j)));
}

void Acre::draw(const GLuint& worldMatrixLocation, const GLuint& colorLocation) {
	this->platform.draw(worldMatrixLocation, colorLocation);
	
	for (int i = 0; i < Width; i++)
		for (int j = 0; j < Width; j++)
			tiles[i][j].draw(worldMatrixLocation, colorLocation);
}
