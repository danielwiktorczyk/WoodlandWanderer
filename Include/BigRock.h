#pragma once

#include "CollidableModel.h"
#include "Assets.h"

class BigRock : public CollidableModel {
public:
	BigRock() = default;
	BigRock(GLuint VAO, GLuint numVertices, glm::vec3 color);
	~BigRock();
};
