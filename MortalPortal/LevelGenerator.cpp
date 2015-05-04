#include "LevelGenerator.h"
#include <DirectXMath.h>
#include <time.h>
#include <sstream>

using namespace std;

LevelGenerator::LevelGenerator(std::string pathToFiles, std::string pheaderFile)
{
	std::string pathToLevelHeaderFile = pathToFiles + pheaderFile;
	this->pathToFiles = pathToFiles;

	ifstream in(pathToLevelHeaderFile.c_str());

	if (!in.is_open())
		throw std::runtime_error("Failed to open 'LEVELPARTNAMES.txt'");

	numFiles = 0;
	while (!in.eof())
	{
	   	string fileName;
		getline(in, fileName);
		fileIndex[fileIndex.size()] = fileName;
		numFiles++;
	}

	if (numFiles == 0)
		throw std::runtime_error("'LEVELPARTNAMES.txt' is empty");

	lastLine.type = "e";
	lastLine.position = 0.0f;
	lastLine.velocity = 0.0f;
	lastLine.spawnNext = 1.5f;

	srand(time(NULL));
}

void LevelGenerator::addComet(Geometry* cometsGeometry, Material* cometsMaterial, Shader* cometsShader)
{
	this->cometsGeometry.push_back(cometsGeometry);
	this->cometsMaterial.push_back(cometsMaterial);
	this->cometsShader.push_back(cometsShader);
}

void LevelGenerator::setPlayerOneCrystals(Geometry* Crystal1Geometry, Material* Crystal1Material, Shader* Crystal1Shader, Geometry* Crystal2Geometry, Material* Crystal2Material, Shader* Crystal2Shader)
{
	playerOneCrystalShader[0] = Crystal1Shader;
	playerOneCrystalGeometry[0] = Crystal1Geometry;
	playerOneCrystalMaterial[0] = Crystal1Material;

	playerOneCrystalShader[1] = Crystal2Shader;
	playerOneCrystalGeometry[1] = Crystal2Geometry;
	playerOneCrystalMaterial[1] = Crystal2Material;
}

void LevelGenerator::setPlayerTwoCrystals(Geometry* Crystal1Geometry, Material* Crystal1Material, Shader* Crystal1Shader, Geometry* Crystal2Geometry, Material* Crystal2Material, Shader* Crystal2Shader)
{
	playerTwoCrystalShader[0] = Crystal1Shader;
	playerTwoCrystalGeometry[0] = Crystal1Geometry;
	playerTwoCrystalMaterial[0] = Crystal1Material;

	playerTwoCrystalShader[1] = Crystal2Shader;
	playerTwoCrystalGeometry[1] = Crystal2Geometry;
	playerTwoCrystalMaterial[1] = Crystal2Material;
}

void LevelGenerator::Update(EntityHandler* entityHandler, float deltaTime)
{
	while(timeSinceLastSpawn > lastLine.spawnNext)
	{
		float XSpawnPos = 35;
		timeSinceLastSpawn -= lastLine.spawnNext;

		unsigned int tries = 0;
		while(!partFile.is_open() && tries != 100)
		{
			unsigned int newPart = rand() % numFiles;
			std::string tmp = pathToFiles + fileIndex[newPart];
			partFile.open(tmp.c_str());
			tries++;
		}

		if (numFiles == 100)
			throw std::runtime_error("Failed open random file referred from 'LEVELPARTNAMES.txt' 100 times");

		std::string nextLine;
		getline(partFile, nextLine);
		std::stringstream ss(nextLine);
		ss >> lastLine.type;
		ss >> lastLine.velocity;
		ss >> lastLine.position;
		ss >> lastLine.spawnNext;
		//fscanf_s(partFile, "%s %f %f %f", lastLine.type, lastLine.position, lastLine.velocity, lastLine.spawnNext);

		if (lastLine.type == "c")
		{
			unsigned int rnd = rand() % cometsGeometry.size();
			Entity* comet = new MapItem(cometsGeometry[rnd], cometsMaterial[rnd], cometsShader[rnd], MapItem::Comet, Color::BLUE,
				DirectX::XMFLOAT3(XSpawnPos, lastLine.position, 0), DirectX::XMFLOAT3(-lastLine.velocity, 0, 0), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(rand() % 360, rand() % 360, rand() % 360));
			entityHandler->Add(comet);
		}
		else if (lastLine.type == "p11")
		{
			MapItem* crystal = new MapItem(playerOneCrystalGeometry[0], playerOneCrystalMaterial[0], playerOneCrystalShader[0], MapItem::Crystal, Color::BLUE,
				DirectX::XMFLOAT3(XSpawnPos, lastLine.position, 0), DirectX::XMFLOAT3(-lastLine.velocity, 0, 0));
			entityHandler->Add(crystal);
		}
		else if (lastLine.type == "p12")
		{
			Entity* crystal = new MapItem(playerOneCrystalGeometry[1], playerOneCrystalMaterial[1], playerOneCrystalShader[1], MapItem::Crystal, Color::GREEN,
				DirectX::XMFLOAT3(XSpawnPos, lastLine.position, 0), DirectX::XMFLOAT3(-lastLine.velocity, 0, 0));
			entityHandler->Add(crystal);
		}
		else if (lastLine.type == "p21")
		{
			Entity* crystal = new MapItem(playerTwoCrystalGeometry[0], playerTwoCrystalMaterial[0], playerTwoCrystalShader[0], MapItem::Crystal, Color::RED,
				DirectX::XMFLOAT3(XSpawnPos, lastLine.position, 0), DirectX::XMFLOAT3(-lastLine.velocity, 0, 0));
			entityHandler->Add(crystal);
		}
		else if (lastLine.type == "p22")
		{
			Entity* crystal = new MapItem(playerTwoCrystalGeometry[1], playerTwoCrystalMaterial[1], playerTwoCrystalShader[1], MapItem::Crystal, Color::YELLOW,
				DirectX::XMFLOAT3(XSpawnPos, lastLine.position, 0), DirectX::XMFLOAT3(-lastLine.velocity, 0, 0));
			entityHandler->Add(crystal);
		}
		else if (lastLine.type == "e")
		{
			partFile.close();
		}
	}

	timeSinceLastSpawn += deltaTime;
}
