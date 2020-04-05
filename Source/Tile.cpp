#include "../Include/Tile.h"
#include <time.h>

Tile::Tile(glm::vec3 translation) {	
	//this->platform = Model(cubeAsset, red);
	this->occupant = Model(ballTreeAsset, turquoise);
	this->translation = translation;

	/*glm::mat4 platformTransformMatrix = glm::mat4(1.0f);
	platformTransformMatrix = glm::translate(platformTransformMatrix, glm::vec3(0.0f, -0.4f, 0.0f));
	platformTransformMatrix = glm::translate(platformTransformMatrix, translation);
	platformTransformMatrix = glm::scale(platformTransformMatrix, glm::vec3(10.0f, 1.0f, 10.0f));*/

	//this->platform.setModelTransformMatrix(platformTransformMatrix);

	float randScale = 0.8f + (0.2f * (rand() % 100) / 100.0f);
	randScale *= 4.0f;
	float randRotate = (rand() % 100) / 100.0f * 3.14 * 2;
	float randTranslateX = -2.0f + 4.0f * (rand() % 100) / 100.0f;
	float randTransalteY = -2.0f + 4.0f * (rand() % 100) / 100.0f;

	glm::mat4 occupantPosition = glm::mat4(1.0f);
	occupantPosition = glm::translate(occupantPosition, glm::vec3(randTranslateX, 0.0f, randTransalteY));
	occupantPosition = glm::translate(occupantPosition, translation);
	occupantPosition = glm::scale(occupantPosition, glm::vec3(randScale, randScale, randScale));
	occupantPosition = glm::rotate(occupantPosition, randRotate, glm::vec3(0.0f, 1.0f, 0.0f));

	this->occupant.setModelTransformMatrix(occupantPosition);
}

void Tile::draw(GLuint& worldMatrixLocation, GLuint colorLocation) {
	//this->platform.draw(worldMatrixLocation, colorLocation);
	this->occupant.draw(worldMatrixLocation, colorLocation);
}
