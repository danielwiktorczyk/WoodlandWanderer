#include "..\Include\Forest.h"

Forest::Forest() {
	this->VAO = VAO;
	this->VBO = VBO;
	this->vertices = {
		glm::vec3 { 100.0f,  0.0f,  100.0f },
		glm::vec3 { 100.0f,  0.0f, -100.0f },
		glm::vec3 {-100.0f,  0.0f,  100.0f },

		glm::vec3 { 100.0f,  0.0f, -100.0f },
		glm::vec3 {-100.0f,  0.0f, -100.0f },
		glm::vec3 {-100.0f,  0.0f,  100.0f }
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices.front(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Forest::draw(const GLfloat* value4fv, const GLfloat* value3fv, GLuint& worldMatrixLocationColor, GLuint colorLocation) {
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VAO);

	glUniformMatrix4fv(worldMatrixLocationColor, 1, GL_FALSE, value4fv);
	glUniform3fv(colorLocation, 1, value3fv);

	glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());
}


