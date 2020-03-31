#include "../Include/LoadedObject.h"

LoadedObject::LoadedObject(GLuint VAO) {
	this->VAO = VAO;
}

void LoadedObject::draw(const GLfloat* value4fv,
					    const GLfloat* value3fv,
					    GLuint& worldMatrixLocationColor, 
					    GLuint colorLocation, 
					    const int& vertices) {
	
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VAO);

	glUniformMatrix4fv(worldMatrixLocationColor, 1, GL_FALSE, value4fv);
	glUniform3fv(colorLocation, 1, value3fv);
	
	glDrawArrays(GL_TRIANGLES, 0, vertices);
}
