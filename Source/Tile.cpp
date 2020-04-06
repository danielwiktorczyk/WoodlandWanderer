#include "../Include/Tile.h"
#include <time.h>

Tile::Tile(glm::vec3 translation, AllAssets& assets) {
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
	glm::vec3 colour = glm::vec3(rand() % 20 / 100.0f - 0.3f, rand() % 20 / 100.0f - 0.3f, rand() % 20 / 100.0f - 0.3f);
	switch (category) {
		case 0: // Tree
			colour += turquoise;
			switch (subcategory) {
			case 0:
				this->occupant = Model(assets.getBallTree().getVAO(), assets.getBallTree().getVectorSize(), colour);
				break;
			case 1: 
				//this->occupant = Model(skinnyTreeAsset, turquoise); // TODO this breaks
				this->occupant = Model(assets.getBallTree().getVAO(), assets.getBallTree().getVectorSize(), colour);
				break;
			case 2: 
				this->occupant = Model(assets.getPineTree().getVAO(), assets.getPineTree().getVectorSize(), colour);
				break;
			case 3: 
				this->occupant = Model(assets.getPetalTree().getVAO(), assets.getPetalTree().getVectorSize(), colour);
				break;
			}
			break;
		case 1: // Grass
			colour += green;
			switch (subcategory) {
			case 0:
				this->occupant = Model(assets.getGrass1().getVAO(), assets.getGrass1().getVectorSize(), colour);
				break;
			case 1:
				this->occupant = Model(assets.getGrass2().getVAO(), assets.getGrass2().getVectorSize(), colour);
				break;
			case 2:
				this->occupant = Model(assets.getGrass3().getVAO(), assets.getGrass3().getVectorSize(), colour);
				break;
			case 3:
				this->occupant = Model(assets.getGrass4().getVAO(), assets.getGrass4().getVectorSize(), colour);
				break;
			}
			break;
		case 2: // Boulder
			colour += glm::vec3(0.5f, 0.1f, 0.1f);
			switch (subcategory) {
			case 0:
				this->occupant = Model(assets.getBoulderBig().getVAO(), assets.getBoulderBig().getVectorSize(), colour);
				break;
			case 1:
				this->occupant = Model(assets.getBoulderMed().getVAO(), assets.getBoulderMed().getVectorSize(), colour);
				break;
			case 2:
				this->occupant = Model(assets.getBoulderSmall().getVAO(), assets.getBoulderSmall().getVectorSize(), colour);
				break;
			case 3:
				this->occupant = Model(assets.getBoulderTiny().getVAO(), assets.getBoulderTiny().getVectorSize(), colour);
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

void Tile::draw(const GLuint& worldMatrixLocation, const GLuint& colorLocation) {
	//this->platform.draw(worldMatrixLocation, colorLocation);
	this->occupant.draw(worldMatrixLocation, colorLocation);
}
