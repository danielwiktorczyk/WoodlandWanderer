#pragma once
#include "Model.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>

class CollidableModel : public Model {
public:
	CollidableModel(GLuint VAO, GLuint colliderVAO, GLuint numVertices, glm::vec3 color);
	virtual ~CollidableModel();

	void setModelTransformMatrix(glm::mat4 modelTransformMatrix) override;
	void draw(const GLuint& worldMatrixLocation, const GLuint& colorLocation) override;
	glm::mat4 getColliderTransformMatrix() { return colliderTransformMatrix; }
	GLuint getColliderVAO() { return colliderVAO; }
private:
	GLuint colliderVAO;
	glm::mat4 colliderTransformMatrix;
};