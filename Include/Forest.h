#pragma once
#include "Model.h"
#include "Parameters.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Acre.h"
#include "Snowman.h"
#include "Assets.h"

class Forest {
public: 
	Forest(Snowman& snowman);
	void draw(const GLuint& worldMatrixLocationColor, const GLuint& colorLocation);
	std::vector<Model> Forest::getNearbyCollidables();
private:
	Model platform;
	Snowman& snowman;
	std::vector< std::vector<Acre> > acres;
};