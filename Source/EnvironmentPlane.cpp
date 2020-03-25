#include "../Include/EnvironmentPlane.h"

EnvironmentPlane::EnvironmentPlane() {
	this->vertices = {
		glm::vec3 { 2.0f,  0.0f,  2.0f },
		glm::vec3 { 2.0f,  0.0f, -2.0f },
		glm::vec3 {-2.0f,  0.0f,  2.0f },

		glm::vec3 { 2.0f,  0.0f, -2.0f },
		glm::vec3 {-2.0f,  0.0f, -2.0f },
		glm::vec3 {-2.0f,  0.0f,  2.0f }
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

void EnvironmentPlane::draw(Shader& shader) {
	shader.use();
	
	shader.setVec3("objColor", { 0.09f, 0.45f, 0.13f });

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
