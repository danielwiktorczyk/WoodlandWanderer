#include "../Include/Tile.h"
#include <time.h>

Tile::Tile(glm::vec3 translation) {	
	//this->platform = Model(cubeAsset, red);
	this->translation = translation;

	/*glm::mat4 platformTransformMatrix = glm::mat4(1.0f);
	platformTransformMatrix = glm::translate(platformTransformMatrix, glm::vec3(0.0f, -0.4f, 0.0f));
	platformTransformMatrix = glm::translate(platformTransformMatrix, translation);
	platformTransformMatrix = glm::scale(platformTransformMatrix, glm::vec3(10.0f, 1.0f, 10.0f));*/

	//this->platform.setModelTransformMatrix(platformTransformMatrix);

	// Select Occupant:
	// TODO trivial now, need more flexible weighting
	int category = rand() % 3;
	int subcategory = rand() % 4;
	switch (category) {
		case 0: // Tree
			switch (subcategory) {
			case 0:
				this->occupant = Model(ballTreeAsset, turquoise);
				break;
			case 1: 
				//this->occupant = Model(skinnyTreeAsset, turquoise); // TODO this breaks
				this->occupant = Model(ballTreeAsset, turquoise);
				break;
			case 2: 
				this->occupant = Model(pineTreeAsset, turquoise);
				break;
			case 3: 
				this->occupant = Model(petalTreeAsset, turquoise);
				break;
			}
			break;
		case 1: // Grass
			switch (subcategory) {
			case 0:
				this->occupant = Model(grass1Asset, turquoise);
				break;
			case 1:
				this->occupant = Model(grass2Asset, turquoise);
				break;
			case 2:
				this->occupant = Model(grass3Asset, turquoise);
				break;
			case 3:
				this->occupant = Model(grass4Asset, turquoise);
				break;
			}
			break;
		case 2: // Boulder
			switch (subcategory) {
			case 0:
				this->occupant = Model(boulderBigAsset, turquoise);
				break;
			case 1:
				this->occupant = Model(boulderMedAsset, turquoise);
				break;
			case 2:
				this->occupant = Model(boulderSmallAsset, turquoise);
				break;
			case 3:
				this->occupant = Model(boulderTinyAsset, turquoise);
				break;
			}
			break;
	}


	float randScale = 0.6f + (0.4f * (rand() % 100) / 100.0f);
	randScale *= 4.0f;
	float randRotate = (rand() % 100) / 100.0f * 3.14 * 2;
	float randTranslateX = -3.0f + 6.0f * (rand() % 100) / 100.0f;
	float randTransalteY = -3.0f + 6.0f * (rand() % 100) / 100.0f;

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
