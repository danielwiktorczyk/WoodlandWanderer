#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "OBJloader.h"

#if defined(PLATFORM_OSX)
const std::string basePath = "Models";
#else
	const std::string basePath = "../Assets/Models";
#endif

class Asset {
public:
	Asset() = default;
	Asset(const std::string& path);

	GLuint getVAO()        { return this->VAO;         };
	GLuint getVectorSize() { return this->numVertices; };

private:
	GLuint VAO;
	GLuint numVertices;
};

class AssetsService {
public:
	static AssetsService* getInstance();
	static void initInstance();
	static void resetInstance();

	Asset getBoulderBig()   { return this->boulderBig;   };
	Asset getBoulderMed()   { return this->boulderMed;   };
	Asset getBoulderSmall() { return this->boulderSmall; };

	Asset getGrass1() { return this->grass1; };
	Asset getGrass2() { return this->grass2; };
	Asset getGrass3() { return this->grass3; };

	Asset getBallTree()      { return this->ballTree;      };
	Asset getEdgyPineTree() { return this->edgyPineTree; };
	Asset getStumpyPineTree()     { return this->stumpyPineTree;     };
	Asset getSeussBallTree()      { return this->seussBallTree; };

	Asset getCube()   { return this->cube;   };
	Asset getSphere() { return this->sphere; };

private:
	AssetsService();
	static AssetsService* instance;
	Asset boulderBig;
	Asset boulderMed;
	Asset boulderSmall;

	Asset grass1;
	Asset grass2;
	Asset grass3;
	Asset grass4;

	Asset ballTree;
	Asset edgyPineTree;
	Asset stumpyPineTree;
	Asset seussBallTree;

	Asset cube;
	Asset sphere;
};

const std::string boulderPath = "/Boulders";
const std::string boulderBigAsset = basePath + boulderPath + "/boulders_big.obj";
const std::string boulderMedAsset = basePath + boulderPath + "/boulders_medium.obj";
const std::string boulderSmallAsset = basePath + boulderPath + "/boulders_small.obj";

const std::string grassPath = "/Grass";
const std::string grass1Asset = basePath + grassPath + "/grass-1.obj";
const std::string grass2Asset = basePath + grassPath + "/grass-2.obj";
const std::string grass3Asset = basePath + grassPath + "/grass-3.obj";

const std::string treePath = "/Trees";
const std::string ballTreeAsset = basePath + treePath + "/ball-tree.obj";
const std::string edgyPineTreeAsset = basePath + treePath + "/edgy-pine-tree.obj";
const std::string seussBallTreeAsset = basePath + treePath + "/seuss-ball-tree.obj";
const std::string stumpyPineTreeAsset = basePath + treePath + "/stumpy-pine-tree.obj";

const std::string cubeAsset = basePath + "/cube.obj";
const std::string sphereAsset = basePath + "/sphere.obj";