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
#include "DefaultShader.h"
#include "PowerBar.h"
#include "PowerBarShader.h"
#include "Particles.h"


class Application : public System
{
private:
	D3DHandler* d3dHandler;
	DefaultShader* shader;
	Importer testImporter;
	Input* input;

	EntityHandler* entityHandler;

	Player* player;

	PowerBar* player1Bar;
	PowerBar* player2Bar;
	PowerBarShader* powerBarShader;

	Particle* particle;
public:
	Application(bool fullscreen, bool showCursor, int screenWidth, int screenHeight, LPCWSTR windowTitle);
	~Application();

	bool Update(float deltaTime); //Returns true on quit
	void Render();
};

