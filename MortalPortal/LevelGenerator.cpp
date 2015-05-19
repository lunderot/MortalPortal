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

	string currentType;
	while (!in.eof())
	{
	   	string fileName;
		getline(in, fileName);
		if (!strcmp(fileName.c_str(), "normalPart"))
			currentType = "normalPart";
		else if (!strcmp(fileName.c_str(), "freenzyPart"))
			currentType = "freenzyPart";

		else if (!strcmp(currentType.c_str(), "normalPart") && !fileName.empty())
			normalLevelParts[normalLevelParts.size()] = fileName;
		else if (!strcmp(currentType.c_str(), "freenzyPart") && !fileName.empty())
			crystalFreenzyParts[crystalFreenzyParts.size()] = fileName;
	}

	if (normalLevelParts.size() == 0 && crystalFreenzyParts.size() == 0)
		throw std::runtime_error("'LEVELPARTNAMES.txt' is empty");

	lastLine.type = "e";
	lastLine.position = 0.0f;
	lastLine.velocity = 0.0f;
	lastLine.spawnNext = 1.5f;

	timeSinceLastFileSpawn = 0.0f;
	timeSinceBackgroundAssetSpawn = 28.0f;

	srand(time(NULL));
}

void LevelGenerator::addComet(Geometry* cometsGeometry, Material* cometsMaterial, Shader* cometsShader)
{
	this->cometsGeometry.push_back(cometsGeometry);
	this->cometsMaterial.push_back(cometsMaterial);
	this->cometsShader.push_back(cometsShader);
}

void LevelGenerator::addBackgroundAsset(Geometry* backgroundAssetGeometry, Material* backgroundAssetMaterial, Shader* AssetShader)
{
	this->backgroundAssetGeometry.push_back(backgroundAssetGeometry);
	this->backgroundAssetMaterial.push_back(backgroundAssetMaterial);
	this->backgroundAssetShader.push_back(AssetShader);
}

void LevelGenerator::setPowerUp(Geometry* powerUpGeometry, Material* powerUpMaterial, Shader* powerUpShader)
{
	//this->powerUpGeometry.push_back(powerUpGeometry);
	//this->powerUpMaterial.push_back(powerUpMaterial);
	//this->powerUpShader.push_back(powerUpShader);
	this->powerUpGeometry = powerUpGeometry;
	this->powerUpMaterial = powerUpMaterial;
	this->powerUpShader = powerUpShader;
}

void LevelGenerator::setPlayerOneCrystals(Geometry* Crystal1Geometry, Material* Crystal1Material, Shader* Crystal1Shader, Geometry* Crystal2Geometry, Material* Crystal2Material, Shader* Crystal2Shader, Geometry* Glow1, Geometry* Glow2, Material* Glow1Material, Material* Glow2Material, Shader* GlowShader)
{
	playerOneCrystalShader[0] = Crystal1Shader;
	playerOneCrystalGeometry[0] = Crystal1Geometry;
	playerOneCrystalMaterial[0] = Crystal1Material;

	playerOneCrystalShader[1] = Crystal2Shader;
	playerOneCrystalGeometry[1] = Crystal2Geometry;
	playerOneCrystalMaterial[1] = Crystal2Material;

	playerOneCrystalShader[2] = GlowShader;
	playerOneCrystalGeometry[2] = Glow1;
	playerOneCrystalMaterial[2] = Glow1Material;

	playerOneCrystalGeometry[3] = Glow2;
	playerOneCrystalMaterial[3] = Glow2Material;
}

void LevelGenerator::setPlayerTwoCrystals(Geometry* Crystal1Geometry, Material* Crystal1Material, Shader* Crystal1Shader, Geometry* Crystal2Geometry, Material* Crystal2Material, Shader* Crystal2Shader, Geometry* Glow1, Geometry* Glow2, Material* Glow1Material, Material* Glow2Material, Shader* GlowShader)
{
	playerTwoCrystalShader[0] = Crystal1Shader;
	playerTwoCrystalGeometry[0] = Crystal1Geometry;
	playerTwoCrystalMaterial[0] = Crystal1Material;

	playerTwoCrystalShader[1] = Crystal2Shader;
	playerTwoCrystalGeometry[1] = Crystal2Geometry;
	playerTwoCrystalMaterial[1] = Crystal2Material;

	playerTwoCrystalShader[2] = GlowShader;
	playerTwoCrystalGeometry[2] = Glow1;
	playerTwoCrystalMaterial[2] = Glow1Material;

	playerTwoCrystalGeometry[3] = Glow2;
	playerTwoCrystalMaterial[3] = Glow2Material;
}

void LevelGenerator::Update(EntityHandler* entityHandler, float deltaTime, bool &crystalFrenzy)
{
	while (!partFile.is_open() || crystalFrenzy == true)
	{
		
		if (crystalFrenzy == true)
		{
			partFile.close();
			unsigned int newPart = rand() % crystalFreenzyParts.size();
			std::string tmp = pathToFiles + crystalFreenzyParts[newPart];
			partFile.open(tmp.c_str());
			crystalFrenzy = false;
		}
		else
		{
			unsigned int newPart = rand() % normalLevelParts.size();
			std::string tmp = pathToFiles + normalLevelParts[newPart];
			partFile.open(tmp.c_str());
		}
	}

	if (timeSinceBackgroundAssetSpawn > 5.0f)
	{
		float XSpawnPos = 300;

		timeSinceBackgroundAssetSpawn = 0.0f;
		unsigned int rnd = rand() % backgroundAssetGeometry.size();
		float rnd_AngVelX = (float)(rand() / (RAND_MAX / 0.1f));
		float rnd_AngVelY = (float)(rand() / (RAND_MAX / 0.1f));
		float rnd_AngVelZ = (float)(rand() / (RAND_MAX / 0.1f));
		float rnd_PosZ = (float)(rand() / (RAND_MAX / 65.0f)) + 55.0f;
		float rnd_PosY = (float)(rand() / (RAND_MAX / 100.0f)) - 50.0f;
		float rnd_Vel = (float)(rand() / (RAND_MAX / 10.0f) + 20.0f);
		Entity* backgroundAsset = new BackgroundAsset(backgroundAssetGeometry[rnd], backgroundAssetMaterial[rnd], backgroundAssetShader[rnd],
			DirectX::XMFLOAT3(XSpawnPos, rnd_PosY, rnd_PosZ), DirectX::XMFLOAT3(-rnd_Vel, 0, 0), DirectX::XMFLOAT3(rnd_AngVelX, rnd_AngVelY, rnd_AngVelZ), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3((float)(rand() / (RAND_MAX / DirectX::XM_PI)), (float)(rand() / (RAND_MAX / DirectX::XM_PI)), (float)(rand() / (RAND_MAX / DirectX::XM_PI))), DirectX::XMFLOAT3(1, 1, 1));
		entityHandler->Add(backgroundAsset);
	}
	timeSinceBackgroundAssetSpawn += deltaTime;

	while(timeSinceLastFileSpawn > lastLine.spawnNext)
	{
		float XSpawnPos = 35;
		timeSinceLastFileSpawn -= lastLine.spawnNext;

		std::string nextLine;
		getline(partFile, nextLine);
		std::stringstream ss(nextLine);
		ss >> lastLine.type;
		ss >> lastLine.velocity;
		ss >> lastLine.position;
		ss >> lastLine.spawnNext;

		if (lastLine.type == "c")
		{
			unsigned int rnd = rand() % cometsGeometry.size();
			Entity* comet = new MapItem(cometsGeometry[rnd], cometsMaterial[rnd], cometsShader[rnd], MapItem::Comet, Color::BLUE, nullptr,
				DirectX::XMFLOAT3(XSpawnPos, lastLine.position, 0), DirectX::XMFLOAT3(-lastLine.velocity, 0, 0), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3((float)(rand() / (RAND_MAX / DirectX::XM_PI)), (float)(rand() / (RAND_MAX / DirectX::XM_PI)), (float)(rand() / (RAND_MAX / DirectX::XM_PI))));
			entityHandler->Add(comet);
		}
		else if (lastLine.type == "pu") // power up
		{
			Entity* PowerUp = new MapItem(powerUpGeometry, powerUpMaterial, powerUpShader, MapItem::PowerUp, Color::BLUE, nullptr,
				DirectX::XMFLOAT3(XSpawnPos, lastLine.position, 0), DirectX::XMFLOAT3(-lastLine.velocity, 0, 0), DirectX::XMFLOAT3(0.0, 0.35, 0), DirectX::XMFLOAT3(0, 0, 0));
			entityHandler->Add(PowerUp);
		}
		else if (lastLine.type == "p11")
		{
			MapItem* glow = new MapItem(playerOneCrystalGeometry[2], playerOneCrystalMaterial[2], playerOneCrystalShader[2], MapItem::Crystal, Color::GREEN, nullptr,
				DirectX::XMFLOAT3(XSpawnPos, lastLine.position, 0), DirectX::XMFLOAT3(-lastLine.velocity, 0, 0), DirectX::XMFLOAT3(0, 0, 0));
			entityHandler->Add(glow);
			MapItem* crystal = new MapItem(playerOneCrystalGeometry[0], playerOneCrystalMaterial[0], playerOneCrystalShader[0], MapItem::Crystal, Color::GREEN, glow,
				DirectX::XMFLOAT3(XSpawnPos, lastLine.position, 0), DirectX::XMFLOAT3(-lastLine.velocity, 0, 0), DirectX::XMFLOAT3(0.2,0.1,0));
			entityHandler->Add(crystal);
		}
		else if (lastLine.type == "p12")
		{
			MapItem* glow = new MapItem(playerOneCrystalGeometry[3], playerOneCrystalMaterial[3], playerOneCrystalShader[2], MapItem::Crystal, Color::RED, nullptr,
				DirectX::XMFLOAT3(XSpawnPos, lastLine.position, 0), DirectX::XMFLOAT3(-lastLine.velocity, 0, 0), DirectX::XMFLOAT3(0, 0, 0));
			entityHandler->Add(glow);
			Entity* crystal = new MapItem(playerOneCrystalGeometry[1], playerOneCrystalMaterial[1], playerOneCrystalShader[1], MapItem::Crystal, Color::RED, glow,
				DirectX::XMFLOAT3(XSpawnPos, lastLine.position, 0), DirectX::XMFLOAT3(-lastLine.velocity, 0, 0), DirectX::XMFLOAT3(0.2, 1, 0));
			entityHandler->Add(crystal);

		}
		else if (lastLine.type == "p21")
		{
			MapItem* glow = new MapItem(playerTwoCrystalGeometry[2], playerTwoCrystalMaterial[2], playerTwoCrystalShader[2], MapItem::Crystal, Color::YELLOW, nullptr,
				DirectX::XMFLOAT3(XSpawnPos, lastLine.position, 0), DirectX::XMFLOAT3(-lastLine.velocity, 0, 0), DirectX::XMFLOAT3(0, 0, 0));
			entityHandler->Add(glow);
			Entity* crystal = new MapItem(playerTwoCrystalGeometry[0], playerTwoCrystalMaterial[0], playerTwoCrystalShader[0], MapItem::Crystal, Color::YELLOW, glow,
				DirectX::XMFLOAT3(XSpawnPos, lastLine.position, 0), DirectX::XMFLOAT3(-lastLine.velocity, 0, 0), DirectX::XMFLOAT3(0.2, 1, 0));
			entityHandler->Add(crystal);
		}
		else if (lastLine.type == "p22")
		{
			MapItem* glow = new MapItem(playerTwoCrystalGeometry[3], playerTwoCrystalMaterial[3], playerTwoCrystalShader[2], MapItem::Crystal, Color::BLUE, nullptr,
				DirectX::XMFLOAT3(XSpawnPos, lastLine.position, 0), DirectX::XMFLOAT3(-lastLine.velocity, 0, 0), DirectX::XMFLOAT3(0, 0, 0));
			entityHandler->Add(glow);
			Entity* crystal = new MapItem(playerTwoCrystalGeometry[1], playerTwoCrystalMaterial[1], playerTwoCrystalShader[1], MapItem::Crystal, Color::BLUE, glow,
				DirectX::XMFLOAT3(XSpawnPos, lastLine.position, 0), DirectX::XMFLOAT3(-lastLine.velocity, 0, 0), DirectX::XMFLOAT3(0.2, 1, 0));
			entityHandler->Add(crystal);
		}
		else if (lastLine.type == "e")
		{
			partFile.close();
		}
	}

	timeSinceLastFileSpawn += deltaTime;
}
