#pragma once
#include <iostream>
#include <DirectXMath.h>

#include "System.h"
#include "D3DHandler.h"
#include "Shader.h"
#include "Importer.h"
#include "Input.h"
#include "ControllerInput.h"
#include "KeyboardInput.h"
#include "EntityHandler.h"
#include "Player.h"
#include "PlayerShader.h"
#include "DefaultShader.h"
#include "PowerBar.h"
#include "PowerBarShader.h"
#include "Particles.h"
#include "LevelGenerator.h"

#include "Background.h"
#include "BackgroundShader.h"

#include "ParticleShader.h"
#include "AssetHandler.h"
#include "TextureHandler.h"


class Application : public System
{
private:
	D3DHandler* d3dHandler;
	DefaultShader* shader;
	Importer testImporter;
	Input* input;
	Input* input2;
	EntityHandler* entityHandler;
	AssetHandler* assetHandler;
	TextureHandler* textureHandler;

	Player* player1;
	char player1Keys[5];
	unsigned int player1Test = 0;
	bool player1PreviousButton = false;
	PlayerShader* playerShader;

	Player* player2;
	char player2Keys[5];
	unsigned int player2Test = 1;
	//PlayerShader* playerShader2;

	PowerBar* player1Bar;
	PowerBar* player2Bar;
	PowerBarShader* powerBarShader;

	Particle* particle;

	LevelGenerator* levelGenerator;

	Background* background;
	BackgroundShader* backgShader;



	ParticleShader* particleShader;

public:
	Application(bool fullscreen, bool showCursor, int screenWidth, int screenHeight, LPCWSTR windowTitle);
	~Application();

	bool Update(float deltaTime); //Returns true on quit
	void Render();


};

