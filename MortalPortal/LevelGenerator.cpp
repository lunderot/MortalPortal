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
		throw std::runtime_error("Failed to open level Headerfile");

	while (!in.eof())
	{
	   	string fileName;
		getline(in, fileName);
		fileIndex[fileIndex.size()] = fileName;
		numFiles++;
	}

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

//void LevelGenerator::setPlayerCrystals(Geometry** playerOneGeometry, Material** playerOneMaterial, Geometry** playerTwoGeometry, Material** playerTwoMaterial, Shader* crystalShader)
//{
//	this->playerOneGeometry = playerOneGeometry;
//	this->playerOneMaterial = playerOneMaterial;
//	this->playerTwoGeometry = playerTwoGeometry;
//	this->playerTwoMaterial = playerTwoMaterial;
//	this->crystalShader = crystalShader;
//}

void LevelGenerator::Update(EntityHandler* entityHandler, float deltaTime)
{
	while(timeSinceLastSpawn > lastLine.spawnNext)
	{
		timeSinceLastSpawn -= lastLine.spawnNext;

		if (!partFile.is_open())
		{
			unsigned int newPart = rand() % numFiles;
			std::string teemo = pathToFiles + fileIndex[newPart];
			partFile.open(teemo.c_str());
		}

		if (!partFile.is_open())
		{
			throw std::runtime_error("Failed to Open Level part file");
		}

		std::string tmp;
		getline(partFile, tmp);
		std::stringstream ss(tmp);
		ss >> lastLine.type;
		ss >> lastLine.velocity;
		ss >> lastLine.position;
		ss >> lastLine.spawnNext;
		//fscanf_s(partFile, "%s %f %f %f", lastLine.type, lastLine.position, lastLine.velocity, lastLine.spawnNext);

		if (lastLine.type == "m")
		{
			unsigned int rnd = rand() % cometsGeometry.size();
			Entity* comet = new Comet(cometsGeometry[rnd], cometsMaterial[rnd], cometsShader[rnd], DirectX::XMFLOAT3(0, lastLine.position, 0), DirectX::XMFLOAT3(-lastLine.velocity, 0, 0), DirectX::XMFLOAT3(0, 0, 0));
			entityHandler->Add(comet);
		}
		else if (lastLine.type == "p11")
		{
			//Entity* crystal = new Entity(&playerOneGeometry[1], DirectX::XMFLOAT3(0, lastLine.position, 0), DirectX::XMFLOAT3(-lastLine.velocity, 0, 0), DirectX::XMFLOAT3(0, 0, 0));
			//entityHandler->Add(crystal);
		}
		else if (lastLine.type == "p12")
		{
			//Entity* crystal = new Entity(&playerOneGeometry[1], DirectX::XMFLOAT3(0, lastLine.position, 0), DirectX::XMFLOAT3(-lastLine.velocity, 0, 0), DirectX::XMFLOAT3(0, 0, 0));
			//entityHandler->Add(crystal);
		}
		else if (lastLine.type == "p21")
		{

		}
		else if (lastLine.type == "p22")
		{

		}
		else if (lastLine.type == "e")
		{
			partFile.close();
		}
	}

	timeSinceLastSpawn += deltaTime;


}
