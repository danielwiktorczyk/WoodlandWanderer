#include "../Include/Acre.h"

Acre::Acre(const GLuint& VAO, const GLuint& numVertices, glm::vec3 color, bool rendered) : VAO(VAO), numVertices(numVertices), color(color), rendered(rendered) { }

void Acre::draw(GLuint& worldMatrixLocation, GLuint colorLocation) {
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VAO);

	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &(this->position)[0][0]);
	glUniform3fv(colorLocation, 1, value_ptr(this->color));

	glDrawArrays(GL_TRIANGLES, 0, this->numVertices);
}
