#pragma once
#include <string>

#if defined(PLATFORM_OSX)
const std::string basePath = "Models";
#else
	const std::string basePath = "../Assets/Models";
#endif

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
 
const std::string cubeAsset = basePath  + "/cube.obj";
const std::string sphereAsset = basePath + "/sphere.obj";
