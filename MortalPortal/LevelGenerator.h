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
	void setPlayerOneCrystals(Geometry* Crystal1Geometry, Material* Crystal1Material, Shader* Crystal1Shader, Geometry* Crystal2Geometry, Material* Crystal2Material, Shader* Crystal2Shader);
	void setPlayerTwoCrystals(Geometry* Crystal1Geometry, Material* Crystal1Material, Shader* Crystal1Shader, Geometry* Crystal2Geometry, Material* Crystal2Material, Shader* Crystal2Shader);

	void Update(EntityHandler* entityHandler, float deltaTime);

protected:

	struct lineFormat
	{
		std::string type;
		float velocity;
		float position;
		float spawnNext;
	};

	std::string  pathToFiles;
	unsigned int numFiles;
	std::map<unsigned int, std::string> fileIndex;
	ifstream partFile;
	lineFormat lastLine;
	float timeSinceLastSpawn;

	vector<Geometry*> cometsGeometry;
	vector<Material*> cometsMaterial;
	vector<Shader*>	cometsShader;


	Shader* playerOneCrystalShader[2];
	Geometry* playerOneCrystalGeometry[2];
	Material* playerOneCrystalMaterial[2];

	Shader* playerTwoCrystalShader[2];
	Geometry* playerTwoCrystalGeometry[2];
	Material* playerTwoCrystalMaterial[2];
};