#pragma once
#include "Model.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Tile.h"
#include "../Include/Assets.h"

//#define AcreWidth 3 // for testing and visualizing
#define AcreWidth 9 // for production

class Acre {
public:
	Acre() = default;
	Acre(glm::vec3 translation);
	void draw(const GLuint& worldMatrixLocation, const GLuint& colorLocation);
	void setTranslationMatrix(glm::vec3 translation) { this->translation = translation; };
	void setRendered(bool rendered) { this->rendered = rendered; };
	bool isRendered() { return this->rendered; };
	void setInitialized(bool initialized) { this->initialized = initialized; };
	bool isInitialized() { return this->initialized; };

private:
	Model platform;
	glm::vec3 translation = glm::vec3(1.0f, 1.0f, 1.0f);
	std::vector< std::vector<Tile> > tiles; // M x M square, where M is ODD
	bool rendered;
	bool initialized;
};