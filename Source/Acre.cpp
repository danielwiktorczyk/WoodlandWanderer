#include "../Include/Acre.h"

Acre::Acre(glm::vec3 translation) {
	this->translation = translation;
	this->rendered = false;
	this->initialized = false;

	this->platform = Model(AssetsService::getInstance()->getCube().getVAO(), AssetsService::getInstance()->getCube().getVectorSize(), darkPurple);
	glm::mat4 platformTransformMatrix = glm::mat4(1.0f);
	platformTransformMatrix = glm::translate(platformTransformMatrix, glm::vec3(0.0f, -0.5f, 0.0f));
	platformTransformMatrix = glm::translate(platformTransformMatrix, translation);
	platformTransformMatrix = glm::scale(platformTransformMatrix, glm::vec3(AcreWidth * 10.0f, 1.0f, AcreWidth * 10.0f));
	this->platform.setModelTransformMatrix(platformTransformMatrix);

	this->tiles.resize(AcreWidth);
	for (int i = 0; i < AcreWidth; ++i)
		this->tiles[i].resize(AcreWidth);
	int upperBound = (AcreWidth - 1) / 2 ;
	int lowerBound = -upperBound;
	for (int i = 0; i < AcreWidth; i++) 
		for (int j = 0; j < AcreWidth; j++) 
			tiles[i][j] = Tile(translation + glm::vec3(10.0f * (lowerBound + i), 0.0f, 10.0f * (lowerBound + j)));
}

void Acre::draw(const GLuint& worldMatrixLocation, const GLuint& colorLocation) {
	if (this->rendered) {
		this->platform.draw(worldMatrixLocation, colorLocation);

		for (int i = 0; i < AcreWidth; i++)
			for (int j = 0; j < AcreWidth; j++)
				tiles[i][j].draw(worldMatrixLocation, colorLocation);
	}
}

std::vector<CollidableModel> Acre::getAllCollidables() {
	std::vector<CollidableModel> collidables;
	
	for (int i = 0; i < AcreWidth; i++) {
		for (int j = 0; j < AcreWidth; j++) {
			Model* occupant = tiles[i][j].getOccupant();
			typeid(occupant);
			//if (typeid(occupant) == typeid(CollidableModel)) {
			//	collidables.emplace_back(occupant);
			//}
		}
	}
		
	return collidables;
}
