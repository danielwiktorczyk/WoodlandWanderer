#include "../Include/Tile.h"

Tile::Tile(GLuint tileVAO, glm::mat4 position, LoadedObject& occupant, glm::vec3 color, const GLuint numVertices) : occupant(occupant), numVertices(numVertices) {
	this->tileVAO = tileVAO;
	this->position = position;
	this->color = color;

	// refactor to method after which randomizes it
	// for now, middle:
	this->occupant.setPosition(position);

}

void Tile::draw(GLuint& worldMatrixLocation, GLuint colorLocation) {
	
	// draw tile itself
	glBindVertexArray(this->tileVAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->tileVAO);

	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &(this->position)[0][0]);
	glUniform3fv(colorLocation, 1, value_ptr(this->color));

	glDrawArrays(GL_TRIANGLES, 0, this->numVertices);

}
