#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "Model.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

class CollidableModel : public Model {
public:
	CollidableModel(GLuint VAO, GLuint colliderVAO, GLuint numVertices, glm::vec3 color);
	virtual ~CollidableModel();

	void setModelTransformMatrix(glm::mat4 modelTransformMatrix) override;
	void draw(const GLuint& worldMatrixLocation, const GLuint& colorLocation) override;
	glm::mat4 getColliderTransformMatrix() { return colliderTransformMatrix; }
	GLuint getColliderVAO() { return colliderVAO; }
	glm::vec3 getColliderPosition();
	glm::vec3 getColliderScale();
	glm::vec3 getCollidableDimensions();
private:
	GLuint colliderVAO;
	glm::mat4 colliderTransformMatrix;
};