#include "..\Include\loadedObject.h"

/**
* Loaded Object Constructor
*/
LoadedObject::LoadedObject(const std::string& path) {

	//read the vertex data from the model's OBJ file
	OBJloader::loadOBJ(path.c_str(), this->vertexValues.vertices, this->vertexValues.normals, this->vertexValues.UVs);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexValues.vertices.size() * sizeof(Vertex), &vertexValues.vertices.front(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) (sizeof(glm::vec3)));
	glEnableVertexAttribArray(1);
	
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) (2 * sizeof(glm::vec3)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void LoadedObject::draw(Shader& shader, const glm::vec3& objColor) {
	glCullFace(GL_FRONT);
	shader.use();

	shader.setVec3("objColor", objColor);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glDrawArrays(GL_TRIANGLES, 0, vertexValues.vertices.size());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glCullFace(GL_BACK);
}
