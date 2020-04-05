#include "../Include/Model.h"

Model::Model(const GLuint& VAO, const GLuint& numVertices, glm::vec3 color) : numVertices(numVertices), color(color), VAO(VAO) { }

void Model::draw(GLuint& worldMatrixLocation, GLuint& colorLocation) {
	glDisable(GL_CULL_FACE);
	
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VAO);

	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &(this->modelTransformMatrix)[0][0]);
	glUniform3fv(colorLocation, 1, value_ptr(this->color));
	
	glDrawArrays(GL_TRIANGLES, 0, this->numVertices);
	glEnable(GL_CULL_FACE);
}
