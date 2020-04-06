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
	Asset getBoulderTiny()  { return this->boulderTiny;  };

	Asset getGrass1() { return this->grass1; };
	Asset getGrass2() { return this->grass2; };
	Asset getGrass3() { return this->grass3; };
	Asset getGrass4() { return this->grass4; };

	Asset getBallTree()      { return this->ballTree;      };
	Asset getCurvyPineTree() { return this->curvyPineTree; };
	Asset getPetalTree()     { return this->petalTree;     };
	Asset getPineTree()      { return this->pineTree;      };

	Asset getCube()   { return this->cube;   };
	Asset getSphere() { return this->sphere; };

private:
	AssetsService();
	static AssetsService* instance;
	Asset boulderBig;
	Asset boulderMed;
	Asset boulderSmall;
	Asset boulderTiny;

	Asset grass1;
	Asset grass2;
	Asset grass3;
	Asset grass4;

	Asset ballTree;
	Asset curvyPineTree;
	Asset petalTree;
	Asset pineTree;

	Asset cube;
	Asset sphere;
};

const std::string boulderPath = "/Boulders";
const std::string boulderBigAsset = basePath + boulderPath + "/boulder-big.obj";
const std::string boulderMedAsset = basePath + boulderPath + "/boulder-med.obj";
const std::string boulderSmallAsset = basePath + boulderPath + "/boulder-small.obj";
const std::string boulderTinyAsset = basePath + boulderPath + "/boulder-tiny.obj";

const std::string grassPath = "/Grass";
const std::string grass1Asset = basePath + grassPath + "/grass-1.obj";
const std::string grass2Asset = basePath + grassPath + "/grass-2.obj";
const std::string grass3Asset = basePath + grassPath + "/grass-3.obj";
const std::string grass4Asset = basePath + grassPath + "/grass-4.obj";

const std::string treePath = "/Trees";
const std::string ballTreeAsset = basePath + treePath + "/ball-tree.obj";
const std::string curvyPineTreeAsset = basePath + treePath + "/curvy-pine-tree.obj";
const std::string petalTreeAsset = basePath + treePath + "/petal-tree.obj";
const std::string pineTreeAsset = basePath + treePath + "/pine-tree.obj";
const std::string skinnyTreeAsset = basePath + treePath + "/skinny-tree.obj";

const std::string cubeAsset = basePath + "/cube.obj";
const std::string sphereAsset = basePath + "/sphere.obj";