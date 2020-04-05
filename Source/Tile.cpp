#include "../Include/Tile.h"
#include <time.h>

Tile::Tile(DrawableModel& platform, DrawableModel& occupant, glm::mat4 translationMatrix) : platform(platform), occupant(occupant) {
	this->translationMatrix = translationMatrix;
	glm::mat4 platformTransformMatrix = glm::mat4(1.0f);
	platformTransformMatrix = glm::scale(platformTransformMatrix, glm::vec3(10.0f, 1.0f, 10.0f));
	platformTransformMatrix = glm::translate(platformTransformMatrix, glm::vec3(0.0f, -0.4f, 0.0f));
	platformTransformMatrix = translationMatrix * platformTransformMatrix;
	this->platform.setModelTransformMatrix(platformTransformMatrix);

	// offset and scew the occupant for a more natural spawn
	srand(time(NULL));
	float randScale = 0.8f + (0.2f * (rand() % 100) / 100.0f);
	float randRotate = (rand() % 100) / 100.0f * 3.14 * 2;
	float randTranslateX = -2.0f + 4.0f * (rand() % 100) / 100.0f;
	float randTransalteY = -2.0f + 4.0f * (rand() % 100) / 100.0f;

	glm::mat4 occupantPosition = translationMatrix;
	occupantPosition = glm::scale(occupantPosition, glm::vec3(randScale, randScale, randScale));
	occupantPosition = glm::rotate(occupantPosition, randRotate, glm::vec3(0.0f, 1.0f, 0.0f));
	occupantPosition = glm::translate(occupantPosition, glm::vec3(randTranslateX, 0.0f, randTransalteY));

	this->occupant.setModelTransformMatrix(occupantPosition);

}

void Tile::draw(GLuint& worldMatrixLocation, GLuint colorLocation) {
	
	// draw tile itself
	this->platform.draw(worldMatrixLocation, colorLocation);

	// draw the occupant 
	this->occupant.draw(worldMatrixLocation, colorLocation);

}
