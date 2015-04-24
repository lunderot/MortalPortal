#include "LevelGenerator.h"
#include <DirectXMath.h>
#include <time.h>

using namespace std;

LevelGenerator::LevelGenerator(std::string pathToFiles, std::string pheaderFile)
{
	std::string pathToLevelHeaderFile = pathToFiles + "//" + pheaderFile;

	ifstream in(pathToLevelHeaderFile.c_str());

	if (!in.is_open())
		throw std::runtime_error("Failed to open level Headerfile");

	while (!in.eof())
	{
	   	string fileName;
		getline(in, fileName);
		fileIndex[fileIndex.size()] = fileName;
	}

	srand(time(NULL));
}

void LevelGenerator::setComets(Geometry** cometsGeometry /*, material* meteoritesMaterial*/, unsigned int numComets)
{
	this->cometsGeometry = cometsGeometry;
	this->numComets = numComets;
}

void LevelGenerator::setPlayerOneCrystals(Geometry* playerOneGeometry)
{
	this->playerOneGeometry = playerOneGeometry;
}

void LevelGenerator::setPlayerTwoCrystals(Geometry* playerTwoGeometry)
{
	this->playerTwoGeometry = playerTwoGeometry;
}

void LevelGenerator::Update(EntityHandler* entityHandler, float deltaTime)
{
	while(timeSinceLastSpawn > lastLine.spawnNext)
	{
		if (partFile == nullptr)
		{
			unsigned int newPart = rand() % numFiles;
			fopen_s(&partFile, fileIndex[newPart].c_str(), "r");
		}

		fscanf_s(partFile, "%s %f %f %f", lastLine);

		if (lastLine.type == "m")
		{
			unsigned int rnd = rand() % numComets;
			Entity* comet = new Comet(cometsGeometry[rnd], DirectX::XMFLOAT3(0, lastLine.position, 0), DirectX::XMFLOAT3(-lastLine.velocity, 0, 0), DirectX::XMFLOAT3(0, 0, 0));
			entityHandler->Add(comet);
		}
		else if (lastLine.type == "p11")
		{
			Entity* crystal = new Entity(&playerOneGeometry[1], DirectX::XMFLOAT3(0, lastLine.position, 0), DirectX::XMFLOAT3(-lastLine.velocity, 0, 0), DirectX::XMFLOAT3(0, 0, 0));
			entityHandler->Add(crystal);
		}
		else if (lastLine.type == "p12")
		{
			Entity* crystal = new Entity(&playerOneGeometry[1], DirectX::XMFLOAT3(0, lastLine.position, 0), DirectX::XMFLOAT3(-lastLine.velocity, 0, 0), DirectX::XMFLOAT3(0, 0, 0));
			entityHandler->Add(crystal);
		}
		else if (lastLine.type == "p21")
		{

		}
		else if (lastLine.type == "p22")
		{

		}
		else if (lastLine.type == "e")
		{

		}
	}


}
