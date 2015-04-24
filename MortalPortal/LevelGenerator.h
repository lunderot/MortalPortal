#pragma once
#include "Entity.h"
#include "EntityHandler.h"
#include "Comet.h"

#include <string>
#include <map>
#include <fstream>
#include <vector>

class LevelGenerator
{
public:
	LevelGenerator(std::string pathToFiles, std::string pheaderFile);

	void addComet(Geometry* cometsGeometry, Material* cometsMaterial, Shader* cometsShader);
	void addForPlayerOne(Geometry* playerOneCrystalGeometry, Material* playerOneCrystalMaterial, Shader* playerOneCrystalShader);
	void addForPlayerTow(Geometry* playerTwoCrystalGeometry, Material* playerTwoCrystalMaterial, Shader* playerTwoCrystalShader);

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


	vector<Shader*> playerOneCrystalShader;
	vector<Geometry*> playerOneCrystalGeometry;
	vector<Material*> playerOneCrystalMaterial;

	vector<Shader*> playerTwoCrystalShader;
	vector<Geometry*> playerTwoCrystalGeometry;
	vector<Material*> playerTwoCrystalMaterial;
};