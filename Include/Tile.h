#pragma once
#pragma once
#include "Model.h"
#include "../Include/Colours.h"
#include "../Include/Assets.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>


class Tile {
public: 
	Tile() = default;
	Tile(glm::vec3 translation);
	
	void draw(const GLuint& worldMatrixLocation, const GLuint& colorLocation);

	void setTranslation(glm::vec3 translation) { this->translation = translation; };

private:
	Model platform;
	Model occupant; 
	glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
};