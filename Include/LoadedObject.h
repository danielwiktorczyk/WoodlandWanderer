#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "OBJloader.h"
#include "Shader.h"
#include "Vertex.h"

class LoadedObject {
public:
	LoadedObject(const std::string& path);
	void draw(Shader& shader, const glm::vec3& objColor);

private:
	GLuint VAO = 0;
	GLuint VBO = 0;
	Vertex vertexValues;
};