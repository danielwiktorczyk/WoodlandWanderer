#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <vector>

struct Vertex {
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> UVs;
};