#pragma once
#include <glm/glm.hpp>
#include <cstring>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

bool loadOBJ(const char* path,
			 std::vector<glm::vec3>& out_vertices,
		 	 std::vector<glm::vec3>& out_normals,
			 std::vector<glm::vec2>& out_uvs);

