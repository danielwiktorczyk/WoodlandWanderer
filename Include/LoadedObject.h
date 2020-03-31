#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>

class LoadedObject {
public:
	LoadedObject(GLuint VAO);
	void draw(const GLfloat* value4fv,
			  const GLfloat* value3fv,
			  GLuint& worldMatrixLocationColor,
			  GLuint colorLocation,
			  const int& vertices);

private:
	GLuint VAO;
};

