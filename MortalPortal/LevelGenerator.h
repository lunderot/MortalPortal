#pragma once
#include "Entity.h"
#include "EntityHandler.h"
#include "MapItem.h"

#include <string>
#include <map>
#include <fstream>
#include <vector>

class LevelGenerator
{
public:
	LevelGenerator(std::string pathToFiles, std::string pheaderFile);

	void addComet(Geometry* cometsGeometry, Material* cometsMaterial, Shader* cometsShader);
	void addBackgroundAsset(Geometry* backgroundAssetGeometry, Material* backgroundAssetMaterial, Shader* AssetShader);
	void setPowerUp(Geometry* powerUpGeometry, Material* powerUpMaterial, Shader* powerUpShader);
	void setPlayerOneCrystals(Geometry* Crystal1Geometry, Material* Crystal1Material, Shader* Crystal1Shader, Geometry* Crystal2Geometry, Material* Crystal2Material, Shader* Crystal2Shader);
	void setPlayerTwoCrystals(Geometry* Crystal1Geometry, Material* Crystal1Material, Shader* Crystal1Shader, Geometry* Crystal2Geometry, Material* Crystal2Material, Shader* Crystal2Shader);

	void Update(EntityHandler* entityHandler, float deltaTime, bool &crystalFrenzy);

protected:

	struct lineFormat
	{
		std::string type;
		float velocity;
		float position;
		float spawnNext;
	};

	std::string  pathToFiles;
	std::map<unsigned int, std::string> normalLevelParts;
	std::map<unsigned int, std::string> crystalFreenzyParts;
	ifstream partFile;
	lineFormat lastLine;
	float timeSinceLastFileSpawn;
	float timeSinceBackgroundAssetSpawn;

	vector<Geometry*> cometsGeometry;
	vector<Material*> cometsMaterial;
	vector<Shader*>	cometsShader;

	vector<Geometry*> backgroundAssetGeometry;
	vector<Material*> backgroundAssetMaterial;
	vector<Shader*>	backgroundAssetShader;

	Geometry* powerUpGeometry;
	Material* powerUpMaterial;
	Shader* powerUpShader;

	Shader* playerOneCrystalShader[2];
	Geometry* playerOneCrystalGeometry[2];
	Material* playerOneCrystalMaterial[2];

	Shader* playerTwoCrystalShader[2];
	Geometry* playerTwoCrystalGeometry[2];
	Material* playerTwoCrystalMaterial[2];
};