#include "..\Include\Forest.h"

Forest::Forest(Snowman& snowman) 
	: snowman(snowman) {
	this->platform = Model(AssetsService::getInstance()->getCube().getVAO(), AssetsService::getInstance()->getCube().getVectorSize(), green);

	glm::mat4 platformTransformMatrix = glm::mat4(1.0f);
	platformTransformMatrix = glm::translate(platformTransformMatrix, glm::vec3(0.0f, -3.0f, 0.0f));
	platformTransformMatrix = glm::scale(platformTransformMatrix, glm::vec3(1000.0f, 3.0f, 1000.0f));
	this->platform.setModelTransformMatrix(platformTransformMatrix);

	this->acres.resize(ForestWidth);
	for (int i = 0; i < ForestWidth; ++i)
		this->acres[i].resize(ForestWidth);
}

void Forest::draw(const GLuint& worldMatrixLocation, const GLuint& colorLocation) {
	//this->platform.draw(worldMatrixLocation, colorLocation);

	for (int i = 0; i < ForestWidth; i++)
		for (int j = 0; j < ForestWidth; j++)
			acres[i][j].setRendered(false);

	float x = this->snowman.origin.x;
	float z = this->snowman.origin.z;
	int midpoint = (ForestWidth) / 2;
	int upperBound = (ForestWidth - 1) / 2;
	int lowerBound = -upperBound;
	int snowmanAcreInX = midpoint + 1 + ((x - AcreWidth * TileWidth / 2) / (AcreWidth * TileWidth));
	int snowmanAcreInZ = midpoint + 1 + ((z - AcreWidth * TileWidth / 2) / (AcreWidth * TileWidth));

	for (int i = snowmanAcreInX - 1; i <= snowmanAcreInX + 1; i++) {
		for (int j = snowmanAcreInZ - 1; j <= snowmanAcreInZ + 1; j++) {
			if (!acres[i][j].isInitialized()) {
				acres[i][j] = Acre(glm::vec3(AcreWidth * TileWidth * (i - midpoint), 0.0f, AcreWidth * TileWidth * (j - midpoint)));
				acres[i][j].setInitialized(true);
			}
			acres[i][j].setRendered(true);
		}
	}
		
	for (int i = 0; i < ForestWidth; i++)
		for (int j = 0; j < ForestWidth; j++)
			acres[i][j].draw(worldMatrixLocation, colorLocation);
}


