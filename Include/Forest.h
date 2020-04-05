#pragma once
#include "DrawableModel.h"
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
	Forest(Model& platform);

	void draw(GLuint& worldMatrixLocationColor, GLuint colorLocation);
	
	void setTranslationMatrix(glm::mat4 translationMatrix) { this->translationMatrix = translationMatrix; };

private:
	Model& platform;
	glm::mat4 translationMatrix = glm::mat4(1.0f);
	std::vector<Acre> acres; // will be 20 x 20, square?
};