#include "..\Include\Forest.h"

Forest::Forest(Snowman& snowman) : snowman(snowman) {
	this->platform = Model(cubeAsset, green);

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
			acres[i][j] = Acre(glm::vec3(10.0f * AcreWidth * (lowerBound + i), 0.0f, 10.0f * AcreWidth * (lowerBound + j)));

}

void Forest::draw(const GLuint& worldMatrixLocation, const GLuint& colorLocation) {
	//this->platform.draw(worldMatrixLocation, colorLocation);

	for (int i = 0; i < ForestWidth; i++)
		for (int j = 0; j < ForestWidth; j++)
			acres[i][j].setRendered(false);

	int upperBound = (ForestWidth - 1) / 2;
	int lowerBound = -upperBound;
	int midpoint = (ForestWidth) / 2;

	int activeAcreInX = midpoint + ((int) (this->snowman.origin.x / (AcreWidth * 10.0f)));
	int activeAcreInZ = midpoint  + ((int) (this->snowman.origin.z / (AcreWidth * 10.0f)));
	acres[activeAcreInX][activeAcreInZ].setRendered(true);
	acres[activeAcreInX + 1][activeAcreInZ].setRendered(true);
	acres[activeAcreInX][activeAcreInZ + 1].setRendered(true);
	acres[activeAcreInX - 1][activeAcreInZ].setRendered(true);
	acres[activeAcreInX][activeAcreInZ - 1].setRendered(true);

	for (int i = 0; i < ForestWidth; i++)
		for (int j = 0; j < ForestWidth; j++)
			acres[i][j].draw(worldMatrixLocation, colorLocation);
}


