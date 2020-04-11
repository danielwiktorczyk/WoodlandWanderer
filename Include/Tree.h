#pragma once

#include "CollidableModel.h"
#include "Assets.h"

class Tree : public CollidableModel {
public:
	Tree() = default;
	Tree(GLuint VAO, GLuint numVertices, glm::vec3 color);
	~Tree();
};
