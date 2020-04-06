#pragma once
#include "Model.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Acre.h"

class Forest {
public: 
	Forest(glm::vec3 translatio);

	void draw(const GLuint& worldMatrixLocationColor, const GLuint& colorLocation);
	
	void setTranslationMatrix(glm::vec3 translation) { this->translation = translation; };

private:
	Model platform;
	glm::vec3 translation = glm::vec3(1.0f, 1.0f, 1.0f);
	std::vector<Acre> acres; // will be 20 x 20, square
};