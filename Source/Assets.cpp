#include "../Include/Assets.h"

/**
* Asset constructor
* Takes the path to the asset and loads it to produce an object
*/
Asset::Asset(const std::string& path) {

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> UVs;

	//read the vertex data from the model's OBJ file
	loadOBJ(path.c_str(), vertices, normals, UVs);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Vertex VBO setup
	GLuint vertices_VBO;
	glGenBuffers(1, &vertices_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertices_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Normals VBO setup
	GLuint normals_VBO;
	glGenBuffers(1, &normals_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, normals_VBO);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);

	//UVs VBO setup
	GLuint uvs_VBO;
	glGenBuffers(1, &uvs_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, uvs_VBO);
	glBufferData(GL_ARRAY_BUFFER,UVs.size() * sizeof(glm::vec2), &UVs.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	this->VAO = VAO;
	this->numVertices = vertices.size();
}

/**
* AllAssets constructor
* Constructs all of the objects at runtime to prevent access errors.
* Stores them in a single object to make increase legibility in the other files
*/
AllAssets::AllAssets() {
	this->boulderBig    = Asset(boulderBigAsset);
	this->boulderMed    = Asset(boulderMedAsset);
	this->boulderSmall  = Asset(boulderSmallAsset);
	this->boulderTiny   = Asset(boulderTinyAsset);

	this->grass1        = Asset(grass1Asset);
	this->grass2        = Asset(grass2Asset);
	this->grass3        = Asset(grass3Asset);
	this->grass4        = Asset(grass4Asset);

	this->ballTree      = Asset(ballTreeAsset);
	this->curvyPineTree = Asset(curvyPineTreeAsset);
	this->petalTree     = Asset(petalTreeAsset);
	this->pineTree      = Asset(pineTreeAsset);

	this->cube          = Asset(cubeAsset);
	this->sphere        = Asset(sphereAsset);
}