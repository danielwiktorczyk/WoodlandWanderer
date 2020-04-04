#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>

class DrawableModel {
public:
	DrawableModel() = default;
	DrawableModel(const GLuint& VAO, const GLuint& numVertices, glm::vec3 color);

	void draw(GLuint& worldMatrixLocation, GLuint& colorLocation);
	
	glm::mat4 getPosition() { return this->position; };
	void setPosition(glm::mat4 position) { this->position = position; };
	glm::vec3 getColor() { return this->color; };
	void setColor(glm::vec3 color) { this->color = color; };

private:
	const GLuint& VAO;
	glm::mat4 position = glm::mat4(1.0f);
	glm::vec3 color;
	const GLuint& numVertices;
};

