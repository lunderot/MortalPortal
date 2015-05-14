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
#include "ButtonShader.h"
#include "ComboBar.h"
#include "ComboBarShader.h"
#include "ComboDisplayText.h"
#include "PowerUpDisplay.h"
#include "Particles.h"
#include "StartMenu.h"
#include "PauseMenu.h"
#include "RestartMenu.h"
#include "LevelGenerator.h"
#include "MapItemShader.h"
#include "RectangleScreen.h"
#include "PowerupIndicator.h"
#include "Background.h"
#include "BackgroundShader.h"
#include "Light.h"

#include "ParticleShader.h"
#include "AssetHandler.h"
#include "TextureHandler.h"

#include "AudioMaster.h"
#include "HighscoreDisplay.h"

class Application : public System
{
private:
	D3DHandler* d3dHandler;
	
	//Shaders
	DefaultShader* shader;
	ComboBarShader* comboBarShader;
	PowerBarShader* powerBarShader;
	ParticleShader* particleShader;
	PlayerShader* playerShader;
	BackgroundShader* backgShader;
	MapItemShader* mapItemShader;
	ButtonShader* buttonShader;

	//Handlers
	Input* input;
	Input* input2;

	EntityHandler* entityHandler;
	AssetHandler* assetHandler;

	AudioMaster aMaster;

	LevelGenerator* levelGenerator;

	//Entities
	Player* player1;
	Player* player2;
	Particle* particle;
	Particle* particle2;
	Particle* particlePowerBar1;
	Particle* particlePowerBar2;
	Particle* particleBackground;
	Particle* particlePortal1;
	Particle* particlePortal2;

	// Particle Materials
	std::vector <Material*> particleMaterials;
	Material* greenParticle;
	Material* redParticle;
	Material* yellowParticle;
	Material* blueParticle;


	//Player data
	char player1Keys[5];
	char player2Keys[5];
	unsigned int player1Test = 0;
	unsigned int player2Test = 1;
	RectangleScreen* playerWins;
	RectangleScreen* player1Info;
	RectangleScreen* player2Info;

	//Power up Inidcators
	PowerupIndicator* slowSpeedIndicator1;
	PowerupIndicator* slowSpeedIndicator2;

	PowerupIndicator* immortalIndicator1;
	PowerupIndicator* immortalIndicator2;

	PowerupIndicator* inverseIndicator1;
	PowerupIndicator* inverseIndicator2;

	// Menu
	StartMenu* startMenu;
	PauseMenu* pauseMenu;
	RestartMenu* restartMenu;
	RectangleScreen* powerbarBackground1;
	RectangleScreen* powerbarBackground2;

	bool crystalFrenzy;
	bool crystalFrenzyControl;

	LightL oneDirectionLightObject;

	HighscoreDisplay* highscoreDisplay;

public:
	Application(bool fullscreen, bool showCursor, int screenWidth, int screenHeight, LPCWSTR windowTitle);
	~Application();

	bool Update(float deltaTime); //Returns true on quit
	void Render();
};

