#include "..\Include\Forest.h"

Forest::Forest(Snowman& snowman, AllAssets& assets) 
	: snowman(snowman) {
	this->platform = Model(assets.getCube().getVAO(), assets.getCube().getVectorSize(), green);

	glm::mat4 platformTransformMatrix = glm::mat4(1.0f);
	platformTransformMatrix = glm::translate(platformTransformMatrix, glm::vec3(0.0f, -3.0f, 0.0f));
	platformTransformMatrix = glm::scale(platformTransformMatrix, glm::vec3(1000.0f, 3.0f, 1000.0f));
	this->platform.setModelTransformMatrix(platformTransformMatrix);

	this->acres.resize(ForestWidth);
	for (int i = 0; i < ForestWidth; ++i)
		this->acres[i].resize(ForestWidth);
	int upperBound = (ForestWidth - 1) / 2;
	int lowerBound = -upperBound;
	int midpoint = (ForestWidth + 1) / 2;
	for (int i = 0; i < ForestWidth; i++)
		for (int j = 0; j < ForestWidth; j++) // TODO not all of them at once!
			acres[i][j] = Acre(glm::vec3(10.0f * AcreWidth * (lowerBound + i), 0.0f, 10.0f * AcreWidth * (lowerBound + j)), assets);

}

void Forest::draw(const GLuint& worldMatrixLocation, const GLuint& colorLocation) {
	//this->platform.draw(worldMatrixLocation, colorLocation);

	for (int i = 0; i < ForestWidth; i++)
		for (int j = 0; j < ForestWidth; j++)
			acres[i][j].setRendered(false);

	int upperBound = (ForestWidth - 1) / 2;
	int lowerBound = -upperBound;
	int midpoint = (ForestWidth) / 2;
	
	float x = this->snowman.origin.x;
	float z = this->snowman.origin.z;
	int activeAcreInX = midpoint + 1 + ((x - AcreWidth * TileWidth / 2) / (AcreWidth * TileWidth));
	int activeAcreInZ = midpoint  + 1 + ((z - AcreWidth * TileWidth / 2) / (AcreWidth * TileWidth));
	acres[activeAcreInX][activeAcreInZ].setRendered(true);
	acres[activeAcreInX][activeAcreInZ + 1].setRendered(true);
	acres[activeAcreInX][activeAcreInZ - 1].setRendered(true);
	acres[activeAcreInX + 1][activeAcreInZ].setRendered(true);
	acres[activeAcreInX + 1][activeAcreInZ + 1].setRendered(true);
	acres[activeAcreInX + 1][activeAcreInZ - 1].setRendered(true);
	acres[activeAcreInX - 1][activeAcreInZ].setRendered(true);
	acres[activeAcreInX - 1][activeAcreInZ + 1].setRendered(true);
	acres[activeAcreInX - 1][activeAcreInZ - 1].setRendered(true);


	for (int i = 0; i < ForestWidth; i++)
		for (int j = 0; j < ForestWidth; j++)
			acres[i][j].draw(worldMatrixLocation, colorLocation);
}


