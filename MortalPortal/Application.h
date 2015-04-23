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

#include "Background.h"
#include "BackgroundShader.h"

#include "ParticleShader.h"
#include "AssetHandler.h"


class Application : public System
{
private:
	D3DHandler* d3dHandler;
	DefaultShader* shader;
	Importer testImporter;
	Input* input;

	EntityHandler* entityHandler;
	AssetHandler* assetHandler;

	Player* player1;
	Player* player2;
	PlayerShader* playerShader;
	PlayerShader* playerShader2;

	PowerBar* player1Bar;
	PowerBar* player2Bar;
	PowerBarShader* powerBarShader;

	Particle* particle;


	Background* background;
	BackgroundShader* backgShader;



	ParticleShader* particleShader;

public:
	Application(bool fullscreen, bool showCursor, int screenWidth, int screenHeight, LPCWSTR windowTitle);
	~Application();

	bool Update(float deltaTime); //Returns true on quit
	void Render();
};

