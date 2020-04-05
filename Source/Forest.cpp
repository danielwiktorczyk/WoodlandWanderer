#include "..\Include\Forest.h"

Forest::Forest(const GLuint& VAO, const GLuint& numVertices, glm::vec3 color) : VAO(VAO), numVertices(numVertices), color(color) { }

void Forest::draw(GLuint& worldMatrixLocationColor, GLuint colorLocation) {
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VAO);

	glUniformMatrix4fv(worldMatrixLocationColor, 1, GL_FALSE, &(this->modelTransformMatrix)[0][0]);
	glUniform3fv(colorLocation, 1, value_ptr(this->color));

	glDrawArrays(GL_TRIANGLES, 0, this->numVertices);
}


