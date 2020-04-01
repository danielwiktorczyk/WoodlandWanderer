#include "../Include/Tile.h"

Tile::Tile(GLuint tileVAO, GLuint occupantVAO, glm::vec3 position) {
	this->occupant = LoadedObject::LoadedObject(occupantVAO);
	this->tileVAO = tileVAO;
	this->position = position;

	// refactor to method after which randomizes it
	// for now, middle:
	this->occupant.setPosition(position);

}

void Tile::draw(const GLfloat* value4fv, const GLfloat* value3fv, GLuint& worldMatrixLocationColor, GLuint colorLocation, const int& vertices) {
	
	// draw tile itself
	glBindVertexArray(this->tileVAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->tileVAO);

	glUniformMatrix4fv(worldMatrixLocationColor, 1, GL_FALSE, value4fv);
	glUniform3fv(colorLocation, 1, value3fv);

	glDrawArrays(GL_TRIANGLES, 0, vertices);

}
