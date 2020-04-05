#include "../Include/Tile.h"
#include <time.h>

Tile::Tile(GLuint tileVAO, glm::mat4 position, DrawableModel& occupant, glm::vec3 color, const GLuint numVertices) : occupant(occupant), numVertices(numVertices) {
	this->tileVAO = tileVAO;
	this->modelTransformMatrix = position;
	this->color = color;

	// offset and scew the occupant for a more natural spawn
	srand(time(NULL));
	float randScale = 0.8f + (0.2f * (rand() % 100) / 100.0f);
	float randRotate = (rand() % 100) / 100.0f * 3.14 * 2;
	float randTranslateX = -2.0f + 4.0f * (rand() % 100) / 100.0f;
	float randTransalteY = -2.0f + 4.0f * (rand() % 100) / 100.0f;

	glm::mat4 occupantPosition = position;
	occupantPosition = glm::scale(occupantPosition, glm::vec3(randScale, randScale, randScale));
	occupantPosition = glm::rotate(occupantPosition, randRotate, glm::vec3(0.0f, 1.0f, 0.0f));
	occupantPosition = glm::translate(occupantPosition, glm::vec3(randTranslateX, 0.0f, randTransalteY));

	this->occupant.setModelTransformMatrix(occupantPosition);

}

void Tile::draw(GLuint& worldMatrixLocation, GLuint colorLocation) {
	
	// draw tile itself
	glBindVertexArray(this->tileVAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->tileVAO);
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &(this->modelTransformMatrix)[0][0]);
	glUniform3fv(colorLocation, 1, value_ptr(this->color));
	glDrawArrays(GL_TRIANGLES, 0, this->numVertices);

	// draw the occupant 
	this->occupant.draw(worldMatrixLocation, colorLocation);

}
