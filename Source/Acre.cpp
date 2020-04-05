#include "../Include/Acre.h"

Acre::Acre(glm::vec3 translation) {
	std::string cubePath = "../Assets/Models/cube.obj";
	std::string spherePath = "../Assets/Models/sphere.obj";
	std::string treePath = "../Assets/Models/Trees/curvy-pine-tree.obj";
	
	this->translation = translation;
	this->platform = Model(cubePath, orange);
	this->rendered = false;

	glm::mat4 platformTransformMatrix = glm::mat4(1.0f);
	platformTransformMatrix = glm::translate(platformTransformMatrix, glm::vec3(0.0f, -0.5f, 0.0f));
	platformTransformMatrix = glm::translate(platformTransformMatrix, translation);
	platformTransformMatrix = glm::scale(platformTransformMatrix, glm::vec3(100.0f, 1.0f, 100.0f));
	this->platform.setModelTransformMatrix(platformTransformMatrix);

	this->tiles.resize(Width);
	for (int i = 0; i < Width; ++i)
		this->tiles[i].resize(Width);

	int upperBound = this->width / 2;
	int lowerBound = -upperBound;
	for (int i = 0; i < this->width - 1; i++) {
		for (int j = 0; j < this->width - 1; j++) {
			glm::vec3 tileTranslation = glm::vec3(10.0f * lowerBound + i * 10.0f, 0.0f, 10.0f * lowerBound + j * 10.0f);
			tiles[i][j] = Tile(tileTranslation + translation);
		}
	}
}

void Acre::draw(GLuint& worldMatrixLocation, GLuint colorLocation) {
	this->platform.draw(worldMatrixLocation, colorLocation);
	
	for (int i = 0; i < this->width - 1; i++) 
		for (int j = 0; j < this->width - 1; j++) 
			tiles[i][j].draw(worldMatrixLocation, colorLocation);
}
