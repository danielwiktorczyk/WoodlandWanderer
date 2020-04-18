#include "../Include/Assets.h"

AssetsService* AssetsService::instance = nullptr;

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
AssetsService::AssetsService() {
	std::cout << "\u001b[36m////////////////////////////////////\n";
	std::cout <<           "/////////// Boulder Assets /////////\n";
	std::cout <<           "////////////////////////////////////\u001b[0m\n\n";

	this->boulderBig    = Asset(boulderBigAsset);
	this->boulderMed    = Asset(boulderMedAsset);
	this->boulderSmall  = Asset(boulderSmallAsset);

	std::cout << "Big Bolder vertices: "    << boulderBig.getVectorSize()   << "\n";
	std::cout << "Medium Bolder vertices: " << boulderMed.getVectorSize()   << "\n";
	std::cout << "Small Bolder vertices: "  << boulderSmall.getVectorSize() << "\n";

	std::cout << "\u001b[32m////////////////////////////////////\n";
	std::cout <<           "//////////// Grass Assets //////////\n";
	std::cout <<           "////////////////////////////////////\u001b[0m\n\n";

	this->grass1        = Asset(grass1Asset);
	this->grass2        = Asset(grass2Asset);
	this->grass3        = Asset(grass3Asset);

	std::cout << "Grass 1 vertices: " << grass1.getVectorSize() << "\n";
	std::cout << "Grass 2 vertices: " << grass2.getVectorSize() << "\n";
	std::cout << "Grass 3 vertices: " << grass3.getVectorSize() << "\n";

	std::cout << "\u001b[35m////////////////////////////////////\n";
	std::cout <<           "//////////// Trees Assets //////////\n";
	std::cout <<           "////////////////////////////////////\u001b[0m\n\n";

	this->ballTree      = Asset(ballTreeAsset);
	this->edgyPineTree = Asset(edgyPineTreeAsset);
	this->stumpyPineTree     = Asset(stumpyPineTreeAsset);
	this->seussBallTree      = Asset(seussBallTreeAsset);

	std::cout << "Ball Tree vertices: "       << ballTree.getVectorSize()      << "\n";
	std::cout << "Curvy Pine Tree vertices: " << edgyPineTree.getVectorSize() << "\n";
	std::cout << "Petal Tree vertices: "      << stumpyPineTree.getVectorSize()     << "\n";
	std::cout << "Pine Tree vertices: "       << seussBallTree.getVectorSize()      << "\n\n";

	std::cout << "\u001b[33m////////////////////////////////////\n";
	std::cout <<           "//////// Miscellaneous Assets //////\n";
	std::cout <<           "////////////////////////////////////\u001b[0m\n\n";

	this->cube          = Asset(cubeAsset);
	this->sphere        = Asset(sphereAsset);

	std::cout << "Cube vertices: "   << cube.getVectorSize()   << "\n";
	std::cout << "Sphere vertices: " << sphere.getVectorSize() << "\n\n";
}

/**
* Create an instance of AssetsService if one does not already exist
*/
void AssetsService::initInstance() {
	if (instance == nullptr) {
		instance = new AssetsService();
	}
	else {
		std::cout << "Tried to create an instance of AssetsService, but one already exists!\n";
	}
}

/**
* Getter
* @return AssetsService instance if it exists. Otherwise, returns nullptr.
*/
AssetsService* AssetsService::getInstance() {
	if (instance == nullptr) {
		std::cout << "AssetsService is not initialized. Please create AssetsService first\n";
		return nullptr;
	}
	else {
		return instance;
	}
}

/**
* Destroy instance
*/
void AssetsService::resetInstance() {
	delete instance;
	instance = nullptr;
}