#include "Application.h"

using namespace DirectX;

Application::Application(bool fullscreen, bool showCursor, int screenWidth, int screenHeight, LPCWSTR windowTitle) : System(fullscreen, showCursor, screenWidth, screenHeight, windowTitle)
{
	//Setup DirectX
	float screenFar = 1000.0f;
	float screenNear = 0.1f;
	d3dHandler = new D3DHandler(screenWidth, screenHeight, hwnd, fullscreen, screenFar, screenNear);

	//Object shaders
	playerShader = new ObjectShader(d3dHandler->GetDevice(), L"assets/shaders/playerVS.hlsl", L"assets/shaders/playerPS.hlsl", screenWidth, screenHeight, screenNear, screenFar);
	mapItemShader = new ObjectShader(d3dHandler->GetDevice(), L"assets/shaders/MapItemVS.hlsl", L"assets/shaders/MapItemPS.hlsl", screenWidth, screenHeight, screenNear, screenFar);
	backgShader = new ObjectShader(d3dHandler->GetDevice(), L"assets/shaders/BackgroundVertexShader.hlsl", L"assets/shaders/BackgroundPixelShader.hlsl", screenWidth, screenHeight, screenNear, screenFar);
	
	//HUD shaders
	buttonShader = new HudShader(d3dHandler->GetDevice(), L"assets/shaders/buttonVS.hlsl", L"assets/shaders/buttonPS.hlsl", screenWidth, screenHeight, screenNear, screenFar);
	powerBarShader = new HudShader(d3dHandler->GetDevice(), L"assets/shaders/powerBarVS.hlsl", L"assets/shaders/powerBarPS.hlsl", screenWidth, screenHeight, screenNear, screenFar);
	comboBarShader = new ComboBarShader(d3dHandler->GetDevice(), L"assets/shaders/comboBarVS.hlsl", L"assets/shaders/comboBarPS.hlsl", screenWidth, screenHeight, screenNear, screenFar);

	numberShader = new OrthoHudShader(d3dHandler->GetDevice(), L"assets/shaders/numberVS.hlsl", L"assets/shaders/numberPS.hlsl", screenWidth, screenHeight, screenNear, screenFar);

	//Particle shader
	particleShader = new ParticleShader(L"assets/shaders/particleCS.hlsl", L"assets/shaders/particleGS.hlsl", d3dHandler->GetDevice(), L"assets/shaders/particleVS.hlsl", L"assets/shaders/particlePS.hlsl", screenWidth, screenHeight, screenNear, screenFar);

	//Audio loading
	unsigned int pirate = aMaster.addNewSample(L"assets/audio/pirate.wav", "pirate", true);
	aMaster.addNewSample(L"assets/audio/boing.wav", "boing", false);
	//aMaster.playSample(pirate);
	
	// Player 1 keys
	player1Keys[0] = 'W';
	player1Keys[1] = 'S';
	player1Keys[2] = 'A';
	player1Keys[3] = 'D';
	player1Keys[4] = 'X';

	// Player 2 keys
	player2Keys[0] = 'I';
	player2Keys[1] = 'K';
	player2Keys[2] = 'J';
	player2Keys[3] = 'L';
	player2Keys[4] = 'M';
	
	//Setup input
	try
	{
		input = new ControllerInput(0);
		std::cout << "Using controller input" << std::endl;
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
		std::cout << "Using keyboard input" << std::endl;
		input = new KeyboardInput(player1Keys);
	}

	try
	{
		input2 = new ControllerInput(1);
		std::cout << "Using controller input" << std::endl;

	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
		std::cout << "Using keyboard input" << std::endl;
		input2 = new KeyboardInput(player2Keys);
	}

	//Create assetHandler
	assetHandler = new AssetHandler();

	//Setup entity handler
	entityHandler = new EntityHandler();

	//Setup levelGenerator
	levelGenerator = new LevelGenerator("assets/levelparts/", "LEVELPARTNAMES.txt");

	levelGenerator->setPlayerOneCrystals(assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/crystal_v2.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "assets/crystal_v2.bin", "Crystal_Green"),
		playerShader,
		assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/crystal_v2.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "assets/crystal_v2.bin", "Crystal_Red"),
		playerShader);

	levelGenerator->setPlayerTwoCrystals(assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/crystal_v2.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "assets/crystal_v2.bin", "Crystal_Yellow"),
		playerShader,
		assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/crystal_v2.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "assets/crystal_v2.bin", "Crystal_Blue"),
		playerShader);

	// Add available comets for LevelGenerator to choose from
	levelGenerator->addComet(assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/small_comet_v1.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "assets/small_comet_v1.bin", "Comet"),
		playerShader);

	levelGenerator->addComet(assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/small_comet_v2.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "assets/small_comet_v2.bin", "Comet"),
		playerShader);

	levelGenerator->addComet(assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/small_comet_v3.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "assets/small_comet_v3.bin", "Comet"),
		playerShader);

	levelGenerator->addBackgroundAsset(assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/EscapePod.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "Zbrush_EscapePod.dds", "", 0.0f, DirectX::XMFLOAT3(0.5, 0.5, 0.5), 10.0f, DirectX::XMFLOAT3(0.1, 0.1, 0.1), DirectX::XMFLOAT3(0, 0, 0)),
		playerShader);

	//levelGenerator->addBackgroundAsset(assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/DeadPortal.bin"),
	//	assetHandler->GetMaterial(d3dHandler->GetDevice(), "assets/Comet.bin", "Comet"),
	//	playerShader);

	levelGenerator->addBackgroundAsset(assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/Satellite.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "Zbrush_Satellite.dds", "", 0.0f),
		playerShader);

	levelGenerator->addBackgroundAsset(assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/DeadPortal.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "Zbrush_DeadPortal.dds", "", 0.0f),
		playerShader);

	levelGenerator->addBackgroundAsset(assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/big_comet.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "assets/big_comet.bin", "Comet"),
		playerShader);

	levelGenerator->setPowerUp(assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/PowerUp.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "Zbrush_PowerUp.dds", "", 0.0f),
		mapItemShader);



	//Create player and add it to entity handler
	player1 = new Player(d3dHandler->GetDevice(),
		assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/New_Portal.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "Zbrush_Portal_Green.dds", "", 0.0f, DirectX::XMFLOAT3(0.5, 0.5, 0.5), 10.0f, DirectX::XMFLOAT3(0.1, 0.1, 0.1), DirectX::XMFLOAT3(0, 0, 0)),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "Zbrush_Portal_Red.dds", "", 0.0f, DirectX::XMFLOAT3(0.5, 0.5, 0.5), 10.0f, DirectX::XMFLOAT3(0.1, 0.1, 0.1), DirectX::XMFLOAT3(0, 0, 0)),
		playerShader,

		Color::GREEN, Color::RED,
		XMFLOAT3(-15, 5, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, XM_PIDIV2, 0));

	entityHandler->Add(player1);

	player2 = new Player(d3dHandler->GetDevice(),
		assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/New_Portal.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "Zbrush_Portal_Yellow.dds", "", 0.0f, DirectX::XMFLOAT3(0.5, 0.5, 0.5), 10.0f, DirectX::XMFLOAT3(0.1, 0.1, 0.1), DirectX::XMFLOAT3(0, 0, 0)),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "Zbrush_Portal_Blue.dds", "", 0.0f, DirectX::XMFLOAT3(0.5, 0.5, 0.5), 10.0f, DirectX::XMFLOAT3(0.1, 0.1, 0.1), DirectX::XMFLOAT3(0, 0, 0)),
		playerShader,

		Color::YELLOW, Color::BLUE,
		XMFLOAT3(-15, -5, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, XM_PIDIV2, 0));

	entityHandler->Add(player2);

	// Create Combo-bar player1 & player2
	Material* playerComboMat[9];
	playerComboMat[0] = assetHandler->GetMaterial(d3dHandler->GetDevice(), "0ggr.dds", "", 0.0f);
	playerComboMat[1] = assetHandler->GetMaterial(d3dHandler->GetDevice(), "2ggr.dds", "", 0.0f);
	playerComboMat[2] = assetHandler->GetMaterial(d3dHandler->GetDevice(), "3ggr.dds", "", 0.0f);
	playerComboMat[3] = assetHandler->GetMaterial(d3dHandler->GetDevice(), "4ggr.dds", "", 0.0f);
	playerComboMat[4] = assetHandler->GetMaterial(d3dHandler->GetDevice(), "5ggr.dds", "", 0.0f);
	playerComboMat[5] = assetHandler->GetMaterial(d3dHandler->GetDevice(), "6ggr.dds", "", 0.0f);
	playerComboMat[6] = assetHandler->GetMaterial(d3dHandler->GetDevice(), "7ggr.dds", "", 0.0f);
	playerComboMat[7] = assetHandler->GetMaterial(d3dHandler->GetDevice(), "8ggr.dds", "", 0.0f);
	playerComboMat[8] = assetHandler->GetMaterial(d3dHandler->GetDevice(), "9ggr.dds", "", 0.0f);
	// player1
	player1->comboBar->setMaterial(playerComboMat);
	// player2
	player2->comboBar->setMaterial(playerComboMat);

	// Create Combo-bar Display text player1 & player2
	Material* playerComboDTMat[11];
	playerComboDTMat[0] = assetHandler->GetMaterial(d3dHandler->GetDevice(), "combo.dds", "", 0.0f);
	playerComboDTMat[1] = assetHandler->GetMaterial(d3dHandler->GetDevice(), "zero.dds", "", 0.0f);
	playerComboDTMat[2] = assetHandler->GetMaterial(d3dHandler->GetDevice(), "one.dds", "", 0.0f);
	playerComboDTMat[3] = assetHandler->GetMaterial(d3dHandler->GetDevice(), "two.dds", "", 0.0f);
	playerComboDTMat[4] = assetHandler->GetMaterial(d3dHandler->GetDevice(), "three.dds", "", 0.0f);
	playerComboDTMat[5] = assetHandler->GetMaterial(d3dHandler->GetDevice(), "four.dds", "", 0.0f);
	playerComboDTMat[6] = assetHandler->GetMaterial(d3dHandler->GetDevice(), "five.dds", "", 0.0f);
	playerComboDTMat[7] = assetHandler->GetMaterial(d3dHandler->GetDevice(), "six.dds", "", 0.0f);
	playerComboDTMat[8] = assetHandler->GetMaterial(d3dHandler->GetDevice(), "seven.dds", "", 0.0f);
	playerComboDTMat[9] = assetHandler->GetMaterial(d3dHandler->GetDevice(), "eight.dds", "", 0.0f);
	playerComboDTMat[10] = assetHandler->GetMaterial(d3dHandler->GetDevice(), "nine.dds", "", 0.0f);
	// player1
	player1->comboDisplayText[0]->setMaterial(playerComboDTMat);
	player1->comboDisplayText[1]->setMaterial(playerComboDTMat);
	player1->comboDisplayText[2]->setMaterial(playerComboDTMat);
	// player2
	player2->comboDisplayText[0]->setMaterial(playerComboDTMat);
	player2->comboDisplayText[1]->setMaterial(playerComboDTMat);
	player2->comboDisplayText[2]->setMaterial(playerComboDTMat);


	// Particle Materials
	greenParticle = assetHandler->GetMaterial(d3dHandler->GetDevice(), "GreenPowerParticle.dds", "", 0.0f);
	redParticle = assetHandler->GetMaterial(d3dHandler->GetDevice(), "RedPowerParticle.dds", "", 0.0f);
	yellowParticle = assetHandler->GetMaterial(d3dHandler->GetDevice(), "YellowPowerParticle.dds", "", 0.0f);
	blueParticle = assetHandler->GetMaterial(d3dHandler->GetDevice(), "BluePowerParticle.dds", "", 0.0f);
	particleMaterials.push_back(redParticle);
	particleMaterials.push_back(greenParticle);
	particleMaterials.push_back(blueParticle);
	particleMaterials.push_back(yellowParticle);

	particle = new Particle(1, 35, assetHandler->GetMaterial(d3dHandler->GetDevice(), "particlePew.dds", "", 0.0f), NULL, d3dHandler->GetDevice());
	particle->constantBufferData.lifeTime = 0.5f;
	particle2 = new Particle(1, 35, assetHandler->GetMaterial(d3dHandler->GetDevice(), "particlePew.dds", "", 0.0f), NULL, d3dHandler->GetDevice());
	particle2->constantBufferData.lifeTime = 0.5f;
	particlePowerBar1 = new Particle(2, 100, assetHandler->GetMaterial(d3dHandler->GetDevice(), "energybar.dds", "", 0.0f), NULL, d3dHandler->GetDevice());
	particlePowerBar1->constantBufferData.reset = false;
	particlePowerBar1->constantBufferData.lifeTime = 20;

	particlePowerBar2 = new Particle(2, 100, assetHandler->GetMaterial(d3dHandler->GetDevice(), "energybar1.dds", "", 0.0f), NULL, d3dHandler->GetDevice());
	particlePowerBar2->constantBufferData.reset = false;
	particlePowerBar2->constantBufferData.lifeTime = 20;

	particleBackground = new Particle(3, 200, assetHandler->GetMaterial(d3dHandler->GetDevice(), "particleTest.dds", "", 0.0f), NULL, d3dHandler->GetDevice());
	particleBackground->constantBufferData.position = DirectX::XMFLOAT3(80, 0, 0);
	particleBackground->constantBufferData.reset = false;

	particlePortal1 = new Particle(4, 200, greenParticle, redParticle, d3dHandler->GetDevice());
	particlePortal1->constantBufferData.reset = false;
	particlePortal1->constantBufferData.lifeTime = 0.3f;

	particlePortal2 = new Particle(4, 200, yellowParticle, blueParticle, d3dHandler->GetDevice());
	particlePortal2->constantBufferData.reset = false;
	particlePortal2->constantBufferData.lifeTime = 0.3f;

	particlePortal1Engine = new Particle(5, 100,greenParticle, redParticle, d3dHandler->GetDevice());
	particlePortal1Engine->constantBufferData.reset = false;
	particlePortal1Engine->constantBufferData.lifeTime = 0.3f;

	particlePortal2Engine = new Particle(5, 100,yellowParticle, blueParticle, d3dHandler->GetDevice());
	particlePortal2Engine->constantBufferData.reset = false;
	particlePortal2Engine->constantBufferData.lifeTime = 0.3f;

	// Power Up & Player Indicators
	//player1Plane = new PowerupIndicator(
	//	assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/BackgroundPlane.bin"),
	//	assetHandler->GetMaterial(d3dHandler->GetDevice(), "Player1Plane.dds", "", 0.0f),
	//	backgShader, XMFLOAT3(0, 0, -1.5), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(2, 3, 1)
	//	);
	//entityHandler->Add(player1Plane);


	
	// Create Background
	entityHandler->Add(
		new Background(
			assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/BackgroundPlane.bin"),
			assetHandler->GetMaterial(d3dHandler->GetDevice(), "spaceBackgroundBlue.dds", "", 0.0f),
			backgShader, XMFLOAT3(0, 0, 230), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(400, 225, 1))
		);
	//stars
	entityHandler->Add(
		new Background(
			assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/BackgroundPlane.bin"),
			assetHandler->GetMaterial(d3dHandler->GetDevice(), "spaceAlphaTest.dds", "", 0.0f),
			backgShader, XMFLOAT3(0, 0, 199.9), XMFLOAT3(2, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(337, 190, 1))
		);
	
	entityHandler->Add(
		new Background(
			assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/BackgroundPlane.bin"),
			assetHandler->GetMaterial(d3dHandler->GetDevice(), "spaceAlphaTest.dds", "", 0.0f),
			backgShader, XMFLOAT3(-674, 0, 199.9), XMFLOAT3(2, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(337, 190, 1))
		);

	entityHandler->Add(
		new Background(
			assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/BackgroundPlane.bin"),
			assetHandler->GetMaterial(d3dHandler->GetDevice(), "spaceAsteroider.dds", "", 0.0f),
			backgShader, XMFLOAT3(-674, 0, 199.8), XMFLOAT3(3, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(337, 190, 1))
		);

	entityHandler->Add(
		new Background(
		assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/BackgroundPlane.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "spaceAsteroider.dds", "", 0.0f),
		backgShader, XMFLOAT3(0, 0, 199.8), XMFLOAT3(3, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(337, 190, 1))
		);

	//entityHandler->Add(
	//	new Background(
	//	assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/BackgroundPlane.bin"),
	//	assetHandler->GetMaterial(d3dHandler->GetDevice(), "spaceAsteroider2.dds", "", 0.0f),
	//	backgShader, XMFLOAT3(0, 0, 199.7), XMFLOAT3(1.5, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(337, 190, 1))
	//	);

	// create Earth
	entityHandler->Add(
		new BackgroundAsset(
			assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/Earth.bin"),
			assetHandler->GetMaterial(d3dHandler->GetDevice(), "EarthTexture.dds", "EarthNormalMap.dds", 0.0f, DirectX::XMFLOAT3(0.0, 0.0, 0.0), 10.0f, DirectX::XMFLOAT3(0.1, 0.1, 0.1), DirectX::XMFLOAT3(0.8, 0.8, 0.8), DirectX::XMFLOAT3(0.0, 0.0, 0.0), DirectX::XMFLOAT3(0.0, 0.0, 0.0), "EarthSpecularMap.dds"),
			playerShader, XMFLOAT3(0, 0, 170), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0.43f, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(7, 7, 7))
		);

	//Light
	this->oneDirection = assetHandler->GetLight(d3dHandler->GetDevice(), "assets/Lighto.bin");
	//oneDirectionLightObject.CreateLight(d3dHandler->GetDevice(), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), 0.0f);

	//Setup player bars
	DirectX::XMFLOAT2 player2BarPos[4];
	player2BarPos[0] = DirectX::XMFLOAT2(0.318f, -0.903f);
	player2BarPos[1] = DirectX::XMFLOAT2(0.318f, -0.974f);
	player2BarPos[2] = DirectX::XMFLOAT2(-0.184f, -0.903f);
	player2BarPos[3] = DirectX::XMFLOAT2(-0.184f, -0.974f);
	player2->powerBar->SetPosition(player2BarPos);
	player2->powerBar->SetMaterial(assetHandler->GetMaterial(d3dHandler->GetDevice(), "energybar1.dds", "", 0.0f));
	player1->powerBar->SetMaterial(assetHandler->GetMaterial(d3dHandler->GetDevice(), "energybar.dds", "", 0.0f));
	
	// Screen space rectangle

	buttonPoint points[4] =
	{
		DirectX::XMFLOAT2(-0.2f, 0.4f),
		DirectX::XMFLOAT2(0.0f, 1.0f),

		DirectX::XMFLOAT2(-0.2f, 0.6f),
		DirectX::XMFLOAT2(0.0f, 0.0f),

		DirectX::XMFLOAT2(0.2f, 0.4f),
		DirectX::XMFLOAT2(1.0f, 1.0f),

		DirectX::XMFLOAT2(0.2f, 0.6f),
		DirectX::XMFLOAT2(1.0f, 0.0f)

	};
	playerWins = new RectangleScreen(points, assetHandler->GetMaterial(d3dHandler->GetDevice(), "player1win.dds", "", 0.0f), assetHandler->GetMaterial(d3dHandler->GetDevice(), "player2win.dds", "", 0.0f), d3dHandler->GetDevice());
	buttonPoint player1Points[4] =
	{
		DirectX::XMFLOAT2(-1.0f, 0.85f),
		DirectX::XMFLOAT2(0.0f, 1.0f),

		DirectX::XMFLOAT2(-1.0f, 1.0f),
		DirectX::XMFLOAT2(0.0f, 0.0f),

		DirectX::XMFLOAT2(1.0f, 0.85f),
		DirectX::XMFLOAT2(1.0f, 1.0f),

		DirectX::XMFLOAT2(1.0f, 1.0f),
		DirectX::XMFLOAT2(1.0f, 0.0f)
	};

	player1Info = new RectangleScreen(player1Points, assetHandler->GetMaterial(d3dHandler->GetDevice(), "gui_Player1.dds", "", 0.0f), assetHandler->GetMaterial(d3dHandler->GetDevice(), "player2win.dds", "", 0.0f), d3dHandler->GetDevice());
	buttonPoint player2Points[4] =
	{
		DirectX::XMFLOAT2(-1.0f, -1.0f),
		DirectX::XMFLOAT2(0.0f, 1.0f),

		DirectX::XMFLOAT2(-1.0f, -0.85f),
		DirectX::XMFLOAT2(0.0f, 0.0f),

		DirectX::XMFLOAT2(1.0f, -1.0f),
		DirectX::XMFLOAT2(1.0f, 1.0f),

		DirectX::XMFLOAT2(1.0f, -0.85f),
		DirectX::XMFLOAT2(1.0f, 0.0f)
	};
	player2Info = new RectangleScreen(player2Points, assetHandler->GetMaterial(d3dHandler->GetDevice(), "gui_Player2.dds", "", 0.0f), assetHandler->GetMaterial(d3dHandler->GetDevice(), "player2win.dds", "", 0.0f), d3dHandler->GetDevice());
	
	// Combo bars, player1 & player2
	// Player 1
	DirectX::XMFLOAT2 comboBarUV[4];
	comboBarUV[0] = DirectX::XMFLOAT2(1.0f, 0.0f);
	comboBarUV[1] = DirectX::XMFLOAT2(1.0f, 1.0f);
	comboBarUV[2] = DirectX::XMFLOAT2(0.0f, 0.0f);
	comboBarUV[3] = DirectX::XMFLOAT2(0.0f, 1.0f);

	DirectX::XMFLOAT2 player1Pos[4];
	player1Pos[0] = DirectX::XMFLOAT2(0.568f, 0.97f); // längst upp - höger
	player1Pos[1] = DirectX::XMFLOAT2(0.568f, 0.916f); // längst ner - höger
	player1Pos[2] = DirectX::XMFLOAT2(0.53f, 0.97f); // längst upp - vänster
	player1Pos[3] = DirectX::XMFLOAT2(0.53f, 0.916f); // längst ner - vänster
	
	player1->comboBar->SetPosition(player1Pos);
	player1->comboBar->SetUV(comboBarUV);
	
	// Player 2
	DirectX::XMFLOAT2 player2Pos[4];
	player2Pos[0] = DirectX::XMFLOAT2(0.568f, -0.918f); // längst upp - höger
	player2Pos[1] = DirectX::XMFLOAT2(0.568f, -0.972f); // längst ner - höger
	player2Pos[2] = DirectX::XMFLOAT2(0.53f, -0.918f); // längst upp - vänster
	player2Pos[3] = DirectX::XMFLOAT2(0.53f, -0.972f); // längst ner - vänster	

	player2->comboBar->SetPosition(player2Pos);
	player2->comboBar->SetUV(comboBarUV);

	// Combo-Display text, player1 & player2
	// Player 1 - "COMBO" text
	player1Pos[0] = DirectX::XMFLOAT2(-0.433f, 0.97f); // längst upp - höger
	player1Pos[1] = DirectX::XMFLOAT2(-0.433f, 0.916f); // längst ner - höger
	player1Pos[2] = DirectX::XMFLOAT2(-0.477f, 0.97f); // längst upp - vänster
	player1Pos[3] = DirectX::XMFLOAT2(-0.477f, 0.916f); // längst ner - vänster
	player1->comboDisplayText[0]->SetPosition(player1Pos);
	player1->comboDisplayText[0]->SetUV(comboBarUV);
	player1->comboDisplayText[0]->SetComboText(false);
	// player 1 - "NUMBERS" text | FIRST
	player1Pos[0] = DirectX::XMFLOAT2(-0.462f, 0.97f); // längst upp - höger
	player1Pos[1] = DirectX::XMFLOAT2(-0.462f, 0.916f); // längst ner - höger
	player1Pos[2] = DirectX::XMFLOAT2(-0.506f, 0.97f); // längst upp - vänster
	player1Pos[3] = DirectX::XMFLOAT2(-0.506f, 0.916f); // längst ner - vänster
	player1->comboDisplayText[1]->SetPosition(player1Pos);
	player1->comboDisplayText[1]->SetUV(comboBarUV);
	player1->comboDisplayText[1]->SetComboText(false);
	// player 1 - "NUMBERS" text | SECOND
	player1Pos[0] = DirectX::XMFLOAT2(-0.491f, 0.97f); // längst upp - höger
	player1Pos[1] = DirectX::XMFLOAT2(-0.491f, 0.916f); // längst ner - höger
	player1Pos[2] = DirectX::XMFLOAT2(-0.535f, 0.97f); // längst upp - vänster
	player1Pos[3] = DirectX::XMFLOAT2(-0.535f, 0.916f); // längst ner - vänster
	player1->comboDisplayText[2]->SetPosition(player1Pos);
	player1->comboDisplayText[2]->SetUV(comboBarUV);
	player1->comboDisplayText[2]->SetComboText(false);

	// Player 2 - "COMBO" text
	player2Pos[0] = DirectX::XMFLOAT2(-0.433f, -0.918f); // längst upp - höger
	player2Pos[1] = DirectX::XMFLOAT2(-0.433f, -0.972f); // längst ner - höger
	player2Pos[2] = DirectX::XMFLOAT2(-0.477f, -0.918f); // längst upp - vänster
	player2Pos[3] = DirectX::XMFLOAT2(-0.477f, -0.972f); // längst ner - vänster
	player2->comboDisplayText[0]->SetPosition(player2Pos);
	player2->comboDisplayText[0]->SetUV(comboBarUV);
	player2->comboDisplayText[0]->SetComboText(false);
	// player 2 - "NUMBERS" text | FIRST
	player2Pos[0] = DirectX::XMFLOAT2(-0.462f, -0.918f); // längst upp - höger
	player2Pos[1] = DirectX::XMFLOAT2(-0.462f, -0.972f); // längst ner - höger
	player2Pos[2] = DirectX::XMFLOAT2(-0.506f, -0.918f); // längst upp - vänster
	player2Pos[3] = DirectX::XMFLOAT2(-0.506f, -0.972f); // längst ner - vänster
	player2->comboDisplayText[1]->SetPosition(player2Pos);
	player2->comboDisplayText[1]->SetUV(comboBarUV);
	player2->comboDisplayText[1]->SetComboText(false);
	// player 2 - "NUMBERS" text | SECOND
	player2Pos[0] = DirectX::XMFLOAT2(-0.491f, -0.918f); // längst upp - höger
	player2Pos[1] = DirectX::XMFLOAT2(-0.491f, -0.972f); // längst ner - höger
	player2Pos[2] = DirectX::XMFLOAT2(-0.535f, -0.918f); // längst upp - vänster
	player2Pos[3] = DirectX::XMFLOAT2(-0.535f, -0.972f); // längst ner - vänster
	player2->comboDisplayText[2]->SetPosition(player2Pos);
	player2->comboDisplayText[2]->SetUV(comboBarUV);
	player2->comboDisplayText[2]->SetComboText(false);

	// Start Menu
	startMenu = new StartMenu(d3dHandler->GetDevice());

	startMenu->AddButton(new StartButton(entityHandler,
		player1, player2,
		DirectX::XMFLOAT2(0, 0.2f),
		DirectX::XMFLOAT2(0.1f, 0.1f),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "start.dds", "", 0.0f)));

	startMenu->AddButton(new QuitButton(
		DirectX::XMFLOAT2(0, -0.2f),
		DirectX::XMFLOAT2(0.1f, 0.1f),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "quit.dds", "", 0.0f)));

	// Pause Menu
	pauseMenu = new PauseMenu(d3dHandler->GetDevice());
	pauseMenu->AddButton(new ContinueButton(
		DirectX::XMFLOAT2(0, 0.4f),
		DirectX::XMFLOAT2(0.1f, 0.1f),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "continue.dds", "", 0.0f)));

	pauseMenu->AddButton(new StartButton(entityHandler,
		player1, player2,
		DirectX::XMFLOAT2(0, 0.0f),
		DirectX::XMFLOAT2(0.1f, 0.1f),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "restart.dds", "", 0.0f)));

	pauseMenu->AddButton(new QuitButton(
		DirectX::XMFLOAT2(0, -0.4f),
		DirectX::XMFLOAT2(0.1f, 0.1f),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "quit.dds", "", 0.0f)));

	// Restart Menu
	restartMenu = new RestartMenu(d3dHandler->GetDevice());

	restartMenu->AddButton(new StartButton(entityHandler,
		player1, player2,
		DirectX::XMFLOAT2(0, 0.2f),
		DirectX::XMFLOAT2(0.1f, 0.1f),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "restart.dds", "", 0.0f)));

	restartMenu->AddButton(new QuitButton(
		DirectX::XMFLOAT2(0, -0.2f),
		DirectX::XMFLOAT2(0.1f, 0.1f),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "quit.dds", "", 0.0f)));



	// Skip the shitty menu
	startMenu->renderMenu = true;
	pauseMenu->renderMenu = false;

	// Power Up
	crystalFrenzy = false;
	crystalFrenzyControl = false;

	slowSpeedIndicator1 = new PowerupIndicator(
		assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/BackgroundPlane.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "FrostPower.dds", "", 0.0f),
		backgShader, XMFLOAT3(0, 0, -1.50), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(1.7f, 2.6f, 1)
		);
	slowSpeedIndicator2 = new PowerupIndicator(
		assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/BackgroundPlane.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "FrostPower.dds", "", 0.0f),
		backgShader, XMFLOAT3(0, 0, -1.51), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(1.7f, 2.6f, 1)
		);

	entityHandler->Add(slowSpeedIndicator1);
	entityHandler->Add(slowSpeedIndicator2);

	immortalIndicator1 = new PowerupIndicator(
		assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/BackgroundPlane.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "immortalPower.dds", "", 0.0f),
		backgShader, XMFLOAT3(0, 0, -1.52), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(3, 3, 1)
		);

	immortalIndicator2 = new PowerupIndicator(
		assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/BackgroundPlane.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "immortalPower.dds", "", 0.0f),
		backgShader, XMFLOAT3(0, 0, -1.53), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(3, 3, 1)
		);

	entityHandler->Add(immortalIndicator1);
	entityHandler->Add(immortalIndicator2);

	inverseIndicator1 = new PowerupIndicator(
		assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/BackgroundPlane.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "error.dds", "", 0.0f),
		backgShader, XMFLOAT3(0, 0, -1.54f), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(2, 2, 1)
		);

	inverseIndicator2 = new PowerupIndicator(
		assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/BackgroundPlane.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "error.dds", "", 0.0f),
		backgShader, XMFLOAT3(0, 0, -1.55f), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(2, 2, 1)
		);

	entityHandler->Add(inverseIndicator1);
	entityHandler->Add(inverseIndicator2);


	highscoreDisplay = new HighscoreDisplay(
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "zero.dds", "", 0.0f),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "one.dds", "", 0.0f),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "two.dds", "", 0.0f),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "three.dds", "", 0.0f),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "four.dds", "", 0.0f),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "five.dds", "", 0.0f),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "six.dds", "", 0.0f),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "seven.dds", "", 0.0f),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "eight.dds", "", 0.0f),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "nine.dds", "", 0.0f),
		assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/BackgroundPlane.bin"),
		numberShader,
		entityHandler
		);

	
	entityHandler->Add(new HudDisplay(
		assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/BackgroundPlane.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "gui_Player2.dds", "", 0.0f),
		numberShader,
		XMFLOAT3(-0.01, -0.5 + 0.05, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0.5, 0.05, 0.5)
		)
	);
	entityHandler->Add(new HudDisplay(
		assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/BackgroundPlane.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "gui_Player1.dds", "", 0.0f),
		numberShader,
		XMFLOAT3(-0.01, 0.5 - 0.05, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0.5, 0.05, 0.5)
		)
		);
}

Application::~Application()
{
	delete d3dHandler;
	
	delete powerBarShader;
	delete comboBarShader;
	delete particleShader;
	delete playerShader;
	delete backgShader;
	delete mapItemShader;
	delete buttonShader;

	delete numberShader;

	delete input;
	delete input2;

	delete entityHandler;
	delete assetHandler;

	delete levelGenerator;

	delete particle;
	delete particle2;
	delete particlePowerBar1;
	delete particlePowerBar2;
	delete particleBackground;
	delete particlePortal1;
	delete particlePortal2;
	delete particlePortal1Engine;
	delete particlePortal2Engine;

	delete startMenu;
	delete pauseMenu;
	delete restartMenu;
	delete playerWins;
	delete player1Info;
	delete player2Info;

	delete oneDirection;

	delete highscoreDisplay;
}

bool Application::Update(float deltaTime)
{
	pauseMenu->CheckIfToPause(input->GetButtonStartState());
	if (pauseMenu->renderMenu == true && startMenu->renderMenu == false || restartMenu->renderMenu == true)
	{
		deltaTime = 0;
		playerWins->player1Wins = false;
	}
	
	UpdatePlayerControls(input, player1, player2, immortalIndicator1, inverseIndicator1, slowSpeedIndicator1, particlePortal2Engine, particlePortal2);
	UpdatePlayerControls(input2, player2, player1, immortalIndicator2, inverseIndicator2, slowSpeedIndicator2,particlePortal1Engine, particlePortal1);

	// Check if Crystal Frenzy is true for both Player1 & Player2wdawddsadsadwas
	if (player1->GetCrystalFrenzy() == true || player2->GetCrystalFrenzy() == true)
	{
		player1->setCrystalFrenzy(false);
		player2->setCrystalFrenzy(false);
		crystalFrenzy = true;
	}

	player1->powerBar->Update(deltaTime, d3dHandler->GetDeviceContext());
	player2->powerBar->Update(deltaTime, d3dHandler->GetDeviceContext());

	player1->comboBar->Update(deltaTime);
	player2->comboBar->Update(deltaTime);

	entityHandler->Update(deltaTime, aMaster);

	particleBackground->UpdateParticle(deltaTime, d3dHandler->GetDeviceContext(), particleShader->GetComputeShader());
	// Particles for player 1
	particlePortal1->UpdateParticle(deltaTime, d3dHandler->GetDeviceContext(), particleShader->GetComputeShader());
	particlePortal1->UpdatePosition(player1->GetPosition());
	particlePortal1->UpdateColor(player1->renderParticles, player1->GetColor(), particle, particleMaterials);

	particlePowerBar1->UpdateParticle(deltaTime, d3dHandler->GetDeviceContext(), particleShader->GetComputeShader());
	particlePowerBar1->UpdatePosition(DirectX::XMFLOAT3(player1->powerBar->GetCurrentMaxPosition().x, player1->powerBar->GetCurrentMaxPosition().y, 0.0f));
	

	particlePortal1Engine->UpdateParticle(deltaTime, d3dHandler->GetDeviceContext(), particleShader->GetComputeShader());
	particlePortal1Engine->UpdatePosition(player1->GetPosition());
	particlePortal1Engine->UpdateColor(player1->renderParticles, player1->GetColor(), particle, particleMaterials);

	if (player1->renderParticles == true && particle->particleCounter <= particle->constantBufferData.lifeTime)
	{
		particle->UpdatePosition(player1->GetPosition());
		particle->UpdateParticle(deltaTime, d3dHandler->GetDeviceContext(), particleShader->GetComputeShader());
		particle->constantBufferData.reset = false;
		if (player1->doubleUp == true)
		{
			particle->constantBufferData.reset = true;
			particle->particleCounter = 0;
			player1->doubleUp = false;
		}
		particle->particleCounter += deltaTime;
		if (particle->particleCounter >= particle->constantBufferData.lifeTime)
		{
			particle->constantBufferData.reset = true;
			particle->particleCounter = 0;
			player1->renderParticles = false;
		}
	}

	// Particles for player 2
	particlePowerBar2->UpdateParticle(deltaTime, d3dHandler->GetDeviceContext(), particleShader->GetComputeShader());
	particlePowerBar2->UpdatePosition(DirectX::XMFLOAT3(player2->powerBar->GetCurrentMaxPosition().x, player2->powerBar->GetCurrentMaxPosition().y, 0.0f));
	
	particlePortal2->UpdateParticle(deltaTime, d3dHandler->GetDeviceContext(), particleShader->GetComputeShader());
	particlePortal2->UpdatePosition(player2->GetPosition());
	particlePortal2->UpdateColor(player2->renderParticles, player2->GetColor(), particle2, particleMaterials);
	
	particlePortal2Engine->UpdateParticle(deltaTime, d3dHandler->GetDeviceContext(), particleShader->GetComputeShader());
	particlePortal2Engine->UpdatePosition(player2->GetPosition());
	particlePortal2Engine->UpdateColor(player2->renderParticles, player2->GetColor(), particle2, particleMaterials);
	if (player2->renderParticles == true && particle->particleCounter <= particle2->constantBufferData.lifeTime)
	{
		particle2->UpdatePosition(player2->GetPosition());
		particle2->UpdateParticle(deltaTime, d3dHandler->GetDeviceContext(), particleShader->GetComputeShader());
		particle2->constantBufferData.reset = false;
		if (player2->doubleUp == true)
		{
			particle2->constantBufferData.reset = true;
			particle2->particleCounter = 0;
			player2->doubleUp = false;
		}
		particle2->particleCounter += deltaTime;
		if (particle2->particleCounter >= particle2->constantBufferData.lifeTime)
		{
			particle2->constantBufferData.reset = true;
			particle2->particleCounter = 0;
			player2->renderParticles = false;
		}
	}
	//player1Plane->UpdatePosition(player1->GetPosition());
	slowSpeedIndicator1->UpdatePosition(player2->GetPosition());
	slowSpeedIndicator2->UpdatePosition(player1->GetPosition());

	
	inverseIndicator1->UpdatePosition(XMFLOAT3(player2->GetPosition().x + rand() % 4 - 2, player2->GetPosition().y + rand() % 4 - 2, player2->GetPosition().z));
	inverseIndicator2->UpdatePosition(XMFLOAT3(player1->GetPosition().x + rand() % 4 - 2, player1->GetPosition().y + rand() % 4 - 2, player1->GetPosition().z));

	immortalIndicator1->UpdatePosition(player1->GetPosition());
	immortalIndicator2->UpdatePosition(player2->GetPosition());

	levelGenerator->Update(entityHandler, deltaTime, crystalFrenzy);

	if (startMenu->renderMenu == true)
		startMenu->Update(input->GetButtonUpState(), input->GetButtonDownState(), input->GetButtonEnterState());

	if (pauseMenu->renderMenu == true && startMenu->renderMenu == false && restartMenu->renderMenu == false)
		pauseMenu->Update(input->GetButtonUpState(), input->GetButtonDownState(), input->GetButtonEnterState());

	if (restartMenu->renderMenu == true)
		restartMenu->Update(input->GetButtonUpState(), input->GetButtonDownState(), input->GetButtonEnterState());


	highscoreDisplay->Update(player1->GetScore(), player2->GetScore());



	return false;
}

void Application::Render()
{
	d3dHandler->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	d3dHandler->EnableDepthStencil();
	d3dHandler->EnableAlphaBlendingFewOverlapping();
	//d3dHandler->GetDeviceContext()->PSSetConstantBuffers(0, 1, &oneDirectionLightObject.pointerToBufferL);
	d3dHandler->GetDeviceContext()->PSSetConstantBuffers(0, 1, &this->oneDirection->pointerToBufferL);

	entityHandler->Render(d3dHandler->GetDeviceContext(), d3dHandler);

	particleShader->Use(d3dHandler->GetDeviceContext());
	d3dHandler->DisableDepthStencil();
	d3dHandler->EnableAlphaBlendingSeverlOverlapping();
	particlePortal1->Render(d3dHandler->GetDeviceContext());
	particlePortal2->Render(d3dHandler->GetDeviceContext());

	particlePortal1Engine->Render(d3dHandler->GetDeviceContext());

	particlePortal2Engine->Render(d3dHandler->GetDeviceContext());

	d3dHandler->EnableAlphaBlendingFewOverlapping();
	particleBackground->Render(d3dHandler->GetDeviceContext());



	if (startMenu->renderMenu == false)
	{
		powerBarShader->Use(d3dHandler->GetDeviceContext());
		playerWins->playerWinsText = false;
		player1Info->Render(d3dHandler->GetDeviceContext());
		player2Info->Render(d3dHandler->GetDeviceContext());
		// Particles
		particleShader->Use(d3dHandler->GetDeviceContext());
		particlePowerBar1->Render(d3dHandler->GetDeviceContext());
		particlePowerBar2->Render(d3dHandler->GetDeviceContext());
		if (player1->renderParticles == true)
		{
			particle->Render(d3dHandler->GetDeviceContext());

		}
		if (player2->renderParticles == true)
		{
			particle2->Render(d3dHandler->GetDeviceContext());

		}
		// Combo - Display text
		comboBarShader->Use(d3dHandler->GetDeviceContext());
		for (int i = 0; i < 3; i++)
		{
			player1->comboDisplayText[i]->Render(d3dHandler->GetDeviceContext(), comboBarShader);
			player2->comboDisplayText[i]->Render(d3dHandler->GetDeviceContext(), comboBarShader);
		}

		// Power Bars
		powerBarShader->Use(d3dHandler->GetDeviceContext());

		player1->powerBar->Render(d3dHandler->GetDeviceContext(), powerBarShader);
		player2->powerBar->Render(d3dHandler->GetDeviceContext(), powerBarShader);

		if (player1->powerBar->IsDead() == true || player2->powerBar->IsDead() == true)
		{
			buttonShader->Use(d3dHandler->GetDeviceContext());
			restartMenu->renderMenu = true;
			restartMenu->buttonScale.button = true;
			restartMenu->Render(d3dHandler->GetDeviceContext());

			if (player2->powerBar->IsDead() == true)
				playerWins->player1Wins = true;
			restartMenu->buttonScale.button = false;
			restartMenu->UpdateConstantBuffer(d3dHandler->GetDeviceContext(), &restartMenu->buttonScale);
			playerWins->playerWinsText = true;
			playerWins->Render(d3dHandler->GetDeviceContext());
		}

		comboBarShader->Use(d3dHandler->GetDeviceContext());
		player1->comboBar->Render(d3dHandler->GetDeviceContext(), comboBarShader);
		player2->comboBar->Render(d3dHandler->GetDeviceContext(), comboBarShader);

		highscoreDisplay->SetVisible(true);
	}
	// Menu
	if (startMenu->renderMenu)
	{
		buttonShader->Use(d3dHandler->GetDeviceContext());
		startMenu->Render(d3dHandler->GetDeviceContext());
		highscoreDisplay->SetVisible(false);
	}

	if (pauseMenu->renderMenu && !startMenu->renderMenu && !restartMenu->renderMenu)
	{
		buttonShader->Use(d3dHandler->GetDeviceContext());
		pauseMenu->Render(d3dHandler->GetDeviceContext());
	}

	d3dHandler->EndScene();
}

void Application::UpdatePlayerControls(Input* input, Player* player, Player* enemyPlayer,
	PowerupIndicator* importalPortalIndicator,
	PowerupIndicator* inverseIndicator,
	PowerupIndicator* slowSpeedIndicator,
	Particle* portalEngine,
	Particle* portalParticles)
{
	//React to switch portal button
	if (pauseMenu->renderMenu == false && restartMenu->renderMenu == false)
	{
		player->ReactToInput(input->GetButtonState(), aMaster);
	}
	player->ReactToControl(input->GetDirection(), enemyPlayer->GetInvertControl(), enemyPlayer->GetSlowDownAcceleration());
	//Set player indicators depending on powerups
	importalPortalIndicator->SetVisible(player->GetImmortalPortal());
	inverseIndicator->SetVisible(player->GetInvertControl());
	slowSpeedIndicator->SetVisible(player->GetSlowDownAcceleration());

	// Disable Portal Engine Particles when slowed
	if (player->GetSlowDownAcceleration())
	{
		portalEngine->constantBufferData.reset = true;
		portalParticles->constantBufferData.reset = true;
	}
	else
	{
		portalEngine->constantBufferData.reset = false;
		portalParticles->constantBufferData.reset = false;
	}
}