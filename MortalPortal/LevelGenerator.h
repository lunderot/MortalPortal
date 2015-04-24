#pragma once
#include "Entity.h"
#include "EntityHandler.h"	
#include <string>
#include <map>
#include <fstream>
#include "Comet.h"

class LevelGenerator
{
public:
	LevelGenerator(std::string pathToFiles, std::string pheaderFile);

	void setComets(Geometry* cometsGeometry, Material* cometsMaterial, unsigned int numComets);
	void setPlayerOneCrystals(Geometry* playerOneGeometry, Material* playerOneMaterial);
	void setPlayerTwoCrystals(Geometry* playerTwoGeometry, Material* playerTwoMaterial);

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
	FILE* partFile;
	lineFormat lastLine;
	float timeSinceLastSpawn;

	unsigned int numComets;
	Geometry* cometsGeometry;
	Material* cometsMaterial;
	Shader*
	Geometry* playerOneGeometry;
	Material* playerOneMaterial;

	Geometry* playerTwoGeometry;
	Material* playerTwoMaterial;
};