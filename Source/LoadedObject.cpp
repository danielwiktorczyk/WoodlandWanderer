#include "../Include/LoadedObject.h"

LoadedObject::LoadedObject(GLuint VAO) {
	this->VAO = VAO;
}

void LoadedObject::draw(const GLfloat* objModel,
					    const GLfloat* objColor,
					    GLuint& worldMatrixLocation, 
					    GLuint colorLocation, 
					    const int& vertices) {
	
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VAO);

	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, objModel);
	glUniform3fv(colorLocation, 1, objColor);
	
	glDrawArrays(GL_TRIANGLES, 0, vertices);
}

glm::vec3 LoadedObject::getPosition()
{
	return this->position;
}

void LoadedObject::setPosition(glm::vec3 position)
{
	this-> position = position;
}
