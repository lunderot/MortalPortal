#include "Application.h"

using namespace DirectX;

Application::Application(bool fullscreen, bool showCursor, int screenWidth, int screenHeight, LPCWSTR windowTitle) : System(fullscreen, showCursor, screenWidth, screenHeight, windowTitle)
{
	//Setup DirectX
	float screenFar = 1000.0f;
	float screenNear = 0.1f;
	d3dHandler = new D3DHandler(screenWidth, screenHeight, hwnd, fullscreen, screenFar, screenNear);

	//Create shaders
	shader = new DefaultShader(d3dHandler->GetDevice(), L"assets/shaders/vs.hlsl", L"assets/shaders/ps.hlsl", screenWidth, screenHeight, screenNear, screenFar);
	powerBarShader = new PowerBarShader(d3dHandler->GetDevice(), L"assets/shaders/powerBarVS.hlsl", L"assets/shaders/powerBarPS.hlsl", screenWidth, screenHeight, screenNear, screenFar);
	comboBarShader = new ComboBarShader(d3dHandler->GetDevice(), L"assets/shaders/comboBarVS.hlsl", L"assets/shaders/comboBarPS.hlsl", screenWidth, screenHeight, screenNear, screenFar);
	playerShader = new PlayerShader(d3dHandler->GetDevice(), L"assets/shaders/playerVS.hlsl", L"assets/shaders/playerPS.hlsl", screenWidth, screenHeight, screenNear, screenFar);

	mapItemShader = new MapItemShader(d3dHandler->GetDevice(), L"assets/shaders/MapItemVS.hlsl", L"assets/shaders/MapItemPS.hlsl", screenWidth, screenHeight, screenNear, screenFar);
	buttonShader = new ButtonShader(d3dHandler->GetDevice(), L"assets/shaders/buttonVS.hlsl", L"assets/shaders/buttonPS.hlsl", screenWidth, screenHeight, screenNear, screenFar);
	// Ayu
	backgShader = new BackgroundShader(d3dHandler->GetDevice(), L"assets/shaders/BackgroundVertexShader.hlsl", L"assets/shaders/BackgroundPixelShader.hlsl", screenWidth, screenHeight, screenNear, screenFar);

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
		input = new ControllerInput(player1Test);
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
		input2 = new ControllerInput(player2Test);
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

	levelGenerator->setPlayerOneCrystals(assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/Player1.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "assets/Player1.bin", "Portal1"),
		mapItemShader,
		assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/Player1.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "assets/Player1.bin", "Portal2"),
		mapItemShader);

	levelGenerator->setPlayerTwoCrystals(assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/Player2Crystal.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "assets/Player2Crystal.bin", "Crystal1"),
		mapItemShader,
		assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/Player2Crystal.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "assets/Player2Crystal.bin", "Crystal2"),
		mapItemShader);

	//Add available comets for LevelGenerator so choose from
	levelGenerator->addComet(assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/Comet.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "2ggr.dds"/*"assets/Comet.bin", "lambert2"*/),
		shader);

	levelGenerator->addPowerUp(assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/Comet.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "b.dds"),
		mapItemShader);


	//Create player and add it to entity handler
	player1 = new Player(d3dHandler->GetDevice(),
		assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/Player1.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "assets/Player1.bin", "Portal1"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "assets/Player1.bin", "Portal2"),
		playerShader,
		Color::BLUE, Color::GREEN,
		XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(40, 0, 30));
	entityHandler->Add(player1);
	player1->SetPlayerNumber(1);

	player2 = new Player(d3dHandler->GetDevice(),
		assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/Player2.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "assets/Player2.bin", "Portal1"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "assets/Player2.bin", "Portal2"),
		playerShader,
		Color::RED, Color::YELLOW,
		XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(40, 0, 30));
	entityHandler->Add(player2);
	player2->SetPlayerNumber(2);

	// Create Combo-bar player1 & player2
	Material* playerComboMat[9];
	playerComboMat[0] = assetHandler->GetMaterial(d3dHandler->GetDevice(), "comboBonus.dds");
	playerComboMat[1] = assetHandler->GetMaterial(d3dHandler->GetDevice(), "2ggr.dds");
	playerComboMat[2] = assetHandler->GetMaterial(d3dHandler->GetDevice(), "3ggr.dds");
	playerComboMat[3] = assetHandler->GetMaterial(d3dHandler->GetDevice(), "4ggr.dds");
	playerComboMat[4] = assetHandler->GetMaterial(d3dHandler->GetDevice(), "5ggr.dds");
	playerComboMat[5] = assetHandler->GetMaterial(d3dHandler->GetDevice(), "6ggr.dds");
	playerComboMat[6] = assetHandler->GetMaterial(d3dHandler->GetDevice(), "7ggr.dds");
	playerComboMat[7] = assetHandler->GetMaterial(d3dHandler->GetDevice(), "8ggr.dds");
	playerComboMat[8] = assetHandler->GetMaterial(d3dHandler->GetDevice(), "9ggr.dds");
	// player1
	player1->comboBar->setMaterial(playerComboMat);
	// player2
	player2->comboBar->setMaterial(playerComboMat);

	// Create Combo-bar Display text player1 & player2
	Material* playerComboDTMat[11];
	playerComboDTMat[0] = assetHandler->GetMaterial(d3dHandler->GetDevice(), "combo.dds");
	playerComboDTMat[1] = assetHandler->GetMaterial(d3dHandler->GetDevice(), "zero.dds");
	playerComboDTMat[2] = assetHandler->GetMaterial(d3dHandler->GetDevice(), "one.dds");
	playerComboDTMat[3] = assetHandler->GetMaterial(d3dHandler->GetDevice(), "two.dds");
	playerComboDTMat[4] = assetHandler->GetMaterial(d3dHandler->GetDevice(), "three.dds");
	playerComboDTMat[5] = assetHandler->GetMaterial(d3dHandler->GetDevice(), "four.dds");
	playerComboDTMat[6] = assetHandler->GetMaterial(d3dHandler->GetDevice(), "five.dds");
	playerComboDTMat[7] = assetHandler->GetMaterial(d3dHandler->GetDevice(), "six.dds");
	playerComboDTMat[8] = assetHandler->GetMaterial(d3dHandler->GetDevice(), "seven.dds");
	playerComboDTMat[9] = assetHandler->GetMaterial(d3dHandler->GetDevice(), "eight.dds");
	playerComboDTMat[10] = assetHandler->GetMaterial(d3dHandler->GetDevice(), "nine.dds");
	// player1
	player1->comboDisplayText[0]->setMaterial(playerComboDTMat);
	player1->comboDisplayText[1]->setMaterial(playerComboDTMat);
	player1->comboDisplayText[2]->setMaterial(playerComboDTMat);
	player1->comboDisplayText[3]->setMaterial(playerComboDTMat);
	// player2
	player2->comboDisplayText[0]->setMaterial(playerComboDTMat);
	player2->comboDisplayText[1]->setMaterial(playerComboDTMat);
	player2->comboDisplayText[2]->setMaterial(playerComboDTMat);
	player2->comboDisplayText[3]->setMaterial(playerComboDTMat);

	// Particles testing area
	particle = new Particle(1, 50, d3dHandler->GetDevice());
	particle2 = new Particle(1, 20, d3dHandler->GetDevice());

	// Create Background
	entityHandler->Add(
		new Background(

		//assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/test.bin"),
		assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/BackgroundPlane.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "spaceTest.dds"),
		playerShader, XMFLOAT3(0, 0, 10), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(46, 26, 1))
		);



	////// create background
	//entityHandler->Add(
	//	new Background(
	//	//assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/test.bin"),
	//	assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/BackgroundPlane.bin"),
	//	assetHandler->GetMaterial(d3dHandler->GetDevice(), "spaceTest.dds"),
	//	playerShader, XMFLOAT3(-91.5, 0, 10), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(46, 26, 1))
	//	);

	//// ---------------- stars alpha

	// Create Background
	entityHandler->Add(
		new Background(

		assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/BackgroundPlane.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "spaceAlphaTest.dds"),
		playerShader, XMFLOAT3(0, 0, 9.9), XMFLOAT3(0.5, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(46, 26, 1))
		);

	entityHandler->Add(
		new Background(
		assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/BackgroundPlane.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "spaceAlphaTest.dds"),
		playerShader, XMFLOAT3(-91.5, 0, 9.9), XMFLOAT3(0.5, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(46, 26, 1))
		);

	entityHandler->Add(
		new Background(
		assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/BackgroundPlane.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "test1.dds"),
		playerShader, XMFLOAT3(-91.5, 0, 9.8), XMFLOAT3(0.4, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(46, 26, 1))
		);

	entityHandler->Add(
		new Background(
		assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/BackgroundPlane.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "test1.dds"),
		playerShader, XMFLOAT3(0, 0, 9.8), XMFLOAT3(0.4, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(46, 26, 1))
		);

	//// ----------------------- end


	// create Earth
	entityHandler->Add(
		new MapItem(
		//assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/test.bin"),
		assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/Earth.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "b.dds"),
		playerShader, MapItem::BackgroundAsset, Color::BLUE, XMFLOAT3(0, 0, 5), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0.05, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1))
		);
	//ljus
	// ljus objekt
	//LightL oneDirectionLightObject;
	oneDirectionLightObject.CreateLight(d3dHandler->GetDevice(), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), float(0.0f) );



	// ------------------ end

	DirectX::XMFLOAT2 player2BarPos[4];
	player2BarPos[0] = DirectX::XMFLOAT2(-0.1f, -0.9f);
	player2BarPos[1] = DirectX::XMFLOAT2(-0.1f, -1.0f);
	player2BarPos[2] = DirectX::XMFLOAT2(-0.7f, -0.9f);
	player2BarPos[3] = DirectX::XMFLOAT2(-0.7f, -1.0f);
	player2->powerBar->SetPosition(player2BarPos);
	player2->powerBar->SetMaterial(assetHandler->GetMaterial(d3dHandler->GetDevice(), "powerbar.dds"));
	player1->powerBar->SetMaterial(assetHandler->GetMaterial(d3dHandler->GetDevice(), "powerbar.dds"));
	// Combo bars, player1 & player2
	// Player 1
	DirectX::XMFLOAT2 player1Pos[4];
	DirectX::XMFLOAT2 player1UV[4];
	player1Pos[0] = DirectX::XMFLOAT2(0.15f, 1.0f); // längst upp - höger
	player1Pos[1] = DirectX::XMFLOAT2(0.15f, 0.8f); // längst ner - höger
	player1Pos[2] = DirectX::XMFLOAT2(0.0f, 1.0f); // längst upp - vänster
	player1Pos[3] = DirectX::XMFLOAT2(0.0f, 0.8f); // längst ner - vänster
	player1UV[0] = DirectX::XMFLOAT2(1.0f, 0.0f);
	player1UV[2] = DirectX::XMFLOAT2(0.0f, 0.0f);
	player1UV[1] = DirectX::XMFLOAT2(1.0f, 1.0f);
	player1UV[3] = DirectX::XMFLOAT2(0.0f, 1.0f);
	player1->comboBar->SetPosition(player1Pos);
	player1->comboBar->SetUV(player1UV);
	
	// Player 2
	DirectX::XMFLOAT2 player2Pos[4];
	DirectX::XMFLOAT2 player2UV[4];
	player2Pos[0] = DirectX::XMFLOAT2(0.15f, -0.8f); // längst upp - höger
	player2Pos[1] = DirectX::XMFLOAT2(0.15f, -1.0f); // längst ner - höger
	player2Pos[2] = DirectX::XMFLOAT2(0.0f, -0.8f); // längst upp - vänster
	player2Pos[3] = DirectX::XMFLOAT2(0.0f, -1.0f); // längst ner - vänster	
	player2UV[0] = DirectX::XMFLOAT2(1.0f, 0.0f);
	player2UV[1] = DirectX::XMFLOAT2(1.0f, 1.0f);
	player2UV[2] = DirectX::XMFLOAT2(0.0f, 0.0f);
	player2UV[3] = DirectX::XMFLOAT2(0.0f, 1.0f);
	player2->comboBar->SetPosition(player2Pos);
	player2->comboBar->SetUV(player2UV);

	// Combo-Display text, player1 & player2
	// Player 1 - "COMBO" text
	player1Pos[0] = DirectX::XMFLOAT2(-0.1f, 0.9f); // längst upp - höger
	player1Pos[1] = DirectX::XMFLOAT2(-0.1f, 0.8f); // längst ner - höger
	player1Pos[2] = DirectX::XMFLOAT2(-0.3f, 0.9f); // längst upp - vänster
	player1Pos[3] = DirectX::XMFLOAT2(-0.3f, 0.8f); // längst ner - vänster
	player1->comboDisplayText[0]->SetPosition(player1Pos);
	player1->comboDisplayText[0]->SetUV(player1UV);
	player1->comboDisplayText[0]->SetComboText(true);
	// player 1 - "NUMBERS" text | FIRST
	player1Pos[0] = DirectX::XMFLOAT2(-0.3f, 0.9f); // längst upp - höger
	player1Pos[1] = DirectX::XMFLOAT2(-0.3f, 0.8f); // längst ner - höger
	player1Pos[2] = DirectX::XMFLOAT2(-0.35f, 0.9f); // längst upp - vänster
	player1Pos[3] = DirectX::XMFLOAT2(-0.35f, 0.8f); // längst ner - vänster
	player1->comboDisplayText[1]->SetPosition(player1Pos);
	player1->comboDisplayText[1]->SetUV(player1UV);
	player1->comboDisplayText[1]->SetComboText(false);
	// player 1 - "NUMBERS" text | SECOND
	player1Pos[0] = DirectX::XMFLOAT2(-0.35f, 0.9f); // längst upp - höger
	player1Pos[1] = DirectX::XMFLOAT2(-0.35f, 0.8f); // längst ner - höger
	player1Pos[2] = DirectX::XMFLOAT2(-0.4f, 0.9f); // längst upp - vänster
	player1Pos[3] = DirectX::XMFLOAT2(-0.4f, 0.8f); // längst ner - vänster
	player1->comboDisplayText[2]->SetPosition(player1Pos);
	player1->comboDisplayText[2]->SetUV(player1UV);
	player1->comboDisplayText[2]->SetComboText(false);
	// player 1 - "NUMBERS" text | THIRD
	player1Pos[0] = DirectX::XMFLOAT2(-0.4f, 0.9f); // längst upp - höger
	player1Pos[1] = DirectX::XMFLOAT2(-0.4f, 0.8f); // längst ner - höger
	player1Pos[2] = DirectX::XMFLOAT2(-0.45f, 0.9f); // längst upp - vänster
	player1Pos[3] = DirectX::XMFLOAT2(-0.45f, 0.8f); // längst ner - vänster
	player1->comboDisplayText[3]->SetPosition(player1Pos);
	player1->comboDisplayText[3]->SetUV(player1UV);
	player1->comboDisplayText[3]->SetComboText(false);

	// Player 2 - "COMBO" text
	player2Pos[0] = DirectX::XMFLOAT2(-0.1f, -0.8f); // längst upp - höger
	player2Pos[1] = DirectX::XMFLOAT2(-0.1f, -0.9f); // längst ner - höger
	player2Pos[2] = DirectX::XMFLOAT2(-0.3f, -0.8f); // längst upp - vänster
	player2Pos[3] = DirectX::XMFLOAT2(-0.3f, -0.9f); // längst ner - vänster
	player2->comboDisplayText[0]->SetPosition(player2Pos);
	player2->comboDisplayText[0]->SetUV(player2UV);
	player2->comboDisplayText[0]->SetComboText(true);
	// player 2 - "NUMBERS" text | FIRST
	player2Pos[0] = DirectX::XMFLOAT2(-0.3f, -0.8f); // längst upp - höger
	player2Pos[1] = DirectX::XMFLOAT2(-0.3f, -0.9f); // längst ner - höger
	player2Pos[2] = DirectX::XMFLOAT2(-0.35f, -0.8f); // längst upp - vänster
	player2Pos[3] = DirectX::XMFLOAT2(-0.35f, -0.9f); // längst ner - vänster
	player2->comboDisplayText[1]->SetPosition(player2Pos);
	player2->comboDisplayText[1]->SetUV(player2UV);
	player2->comboDisplayText[1]->SetComboText(false);
	// player 2 - "NUMBERS" text | SECOND
	player2Pos[0] = DirectX::XMFLOAT2(-0.35f, -0.8f); // längst upp - höger
	player2Pos[1] = DirectX::XMFLOAT2(-0.35f, -0.9f); // längst ner - höger
	player2Pos[2] = DirectX::XMFLOAT2(-0.4f, -0.8f); // längst upp - vänster
	player2Pos[3] = DirectX::XMFLOAT2(-0.4f, -0.9f); // längst ner - vänster
	player2->comboDisplayText[2]->SetPosition(player2Pos);
	player2->comboDisplayText[2]->SetUV(player2UV);
	player2->comboDisplayText[2]->SetComboText(false);
	// player 2 - "NUMBERS" text | THIRD
	player2Pos[0] = DirectX::XMFLOAT2(-0.4f, -0.8f); // längst upp - höger
	player2Pos[1] = DirectX::XMFLOAT2(-0.4f, -0.9f); // längst ner - höger
	player2Pos[2] = DirectX::XMFLOAT2(-0.45f, -0.8f); // längst upp - vänster
	player2Pos[3] = DirectX::XMFLOAT2(-0.45f, -0.9f); // längst ner - vänster
	player2->comboDisplayText[3]->SetPosition(player2Pos);
	player2->comboDisplayText[3]->SetUV(player2UV);
	player2->comboDisplayText[3]->SetComboText(false);

	// Start Menu
	startMenu = new StartMenu(d3dHandler->GetDevice());

	startMenu->AddButton(new StartButton(entityHandler,
		player1, player2,
		DirectX::XMFLOAT2(0, 0.4f),
		DirectX::XMFLOAT2(0.1f, 0.1f),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "start.dds")));

	startMenu->AddButton(new OptionsButton(
		DirectX::XMFLOAT2(0, 0.0f),
		DirectX::XMFLOAT2(0.1f, 0.1f),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "options.dds")));

	startMenu->AddButton(new QuitButton(
		DirectX::XMFLOAT2(0, -0.4f),
		DirectX::XMFLOAT2(0.1f, 0.1f),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "quit.dds")));

	// Pause Menu
	pauseMenu = new PauseMenu(d3dHandler->GetDevice());
	pauseMenu->AddButton(new ContinueButton(
		DirectX::XMFLOAT2(0, 0.4f),
		DirectX::XMFLOAT2(0.1f, 0.1f),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "continue.dds")));

	pauseMenu->AddButton(new StartButton(entityHandler,
		player1, player2,
		DirectX::XMFLOAT2(0, 0.0f),
		DirectX::XMFLOAT2(0.1f, 0.1f),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "restart.dds")));

	pauseMenu->AddButton(new QuitButton(
		DirectX::XMFLOAT2(0, -0.4f),
		DirectX::XMFLOAT2(0.1f, 0.1f),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "quit.dds")));

	// Restart Menu
	restartMenu = new RestartMenu(d3dHandler->GetDevice());

	restartMenu->AddButton(new StartButton(entityHandler,
		player1, player2,
		DirectX::XMFLOAT2(0, 0.2f),
		DirectX::XMFLOAT2(0.1f, 0.1f),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "restart.dds")));

	restartMenu->AddButton(new QuitButton(
		DirectX::XMFLOAT2(0, -0.2f),
		DirectX::XMFLOAT2(0.1f, 0.1f),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "quit.dds")));



	// Skip the shitty menu
	startMenu->renderMenu = false;
	pauseMenu->renderMenu = false;

	// Power Up
	crystalFrenzy = false;
	crystalFrenzyControl = false;
}

Application::~Application()
{
	delete d3dHandler;
	
	delete shader;
	delete powerBarShader;
	delete comboBarShader;
	delete particleShader;
	delete playerShader;
	delete backgShader;
	delete mapItemShader;
	delete buttonShader;

	delete input;
	delete input2;

	delete entityHandler;
	delete assetHandler;

	delete levelGenerator;

	delete particle;
	delete particle2;
	delete startMenu;
	delete pauseMenu;
	delete restartMenu;

}

bool Application::Update(float deltaTime)
{
	pauseMenu->CheckIfToPause(input->GetButtonStartState());
	if (pauseMenu->renderMenu == true && startMenu->renderMenu == false || startMenu->renderMenu == true || restartMenu->renderMenu == true)
	{
		deltaTime = 0;
	}
	// Player 1 - control
	XMFLOAT2 dir = input->GetDirection(player1Test);
	// PowerUp - Invert Control - effect on | Player2
	if (player2->getInvertControl() == true)
	{
		dir.x *= -1;
		dir.y *= -1;
	}
	// PowerUp - Slow Down Acceleration - effect on | Player2
	if (player2->getSlowDownAcceleration() == true)
	{
		dir.x *= 3;
		dir.y *= 3;
	}
	else
	{
		dir.x *= 10;
		dir.y *= 10;
	}
	player1->SetAcceleration(XMFLOAT3(dir.x, dir.y, 0.0f));
	player1->ReactToInput(input->GetButtonState(), aMaster);

	// Player 2 - control
	XMFLOAT2 dir2 = input2->GetDirection(player2Test);
	// PowerUp - Invert Control - effect on | Player1
	if (player1->getInvertControl() == true)
	{
		dir2.x *= -1;
		dir2.y *= -1;
	}
	// PowerUp - Slow Down Acceleration - effect on | Player1
	if (player1->getSlowDownAcceleration() == true)
	{
		dir2.x *= 3;
		dir2.y *= 3;
	}
	else
	{
		dir2.x *= 10;
		dir2.y *= 10;
	}
	player2->SetAcceleration(XMFLOAT3(dir2.x, dir2.y, 0.0f));
	player2->ReactToInput(input2->GetButtonState(), aMaster);
	
	// Check if Crystal Frenzy is true for both Player1 & Player2wdawddsadsadwas
	if (player1->getCrystalFrenzy() == true || player2->getCrystalFrenzy() == true)
	{
		player1->setCrystalFrenzy(false);
		player2->setCrystalFrenzy(false);
		crystalFrenzy = true;
	}

	player1->powerBar->Update(deltaTime, d3dHandler->GetDeviceContext());
	player2->powerBar->Update(deltaTime, d3dHandler->GetDeviceContext());

	player1->comboBar->Update(deltaTime);
	player2->comboBar->Update(deltaTime);

	player1->comboDisplayText[0]->Update(deltaTime);
	player1->comboDisplayText[1]->Update(deltaTime);
	player1->comboDisplayText[2]->Update(deltaTime);
	player1->comboDisplayText[3]->Update(deltaTime);

	player2->comboDisplayText[0]->Update(deltaTime);
	player2->comboDisplayText[1]->Update(deltaTime);
	player2->comboDisplayText[2]->Update(deltaTime);
	player2->comboDisplayText[3]->Update(deltaTime);

	entityHandler->Update(deltaTime, aMaster);

	// Particles for player 1
	particle->UpdatePosition(player1->GetPosition());
	particle->UpdateParticle(deltaTime, d3dHandler->GetDeviceContext(), particleShader->GetComputeShader());
	if (player1->renderParticles == true && particle->particleCounter <= particle->constantBufferData.lifeTime)
	{
		particle->constantBufferData.reset = false;
		if (player1->doubleUp == true)
		{
			particle->constantBufferData.reset = true;
			particle->particleCounter = 0;
			player1->doubleUp = false;
		}
		particle->particleCounter += deltaTime * 20;
		if (particle->particleCounter >= particle->constantBufferData.lifeTime)
		{
			particle->constantBufferData.reset = true;
			particle->particleCounter = 0;
			player1->renderParticles = false;
		}
	}

	// Particles for player 2
	particle2->UpdatePosition(player2->GetPosition());
	particle2->UpdateParticle(deltaTime, d3dHandler->GetDeviceContext(), particleShader->GetComputeShader());
	if (player2->renderParticles == true && particle->particleCounter <= particle2->constantBufferData.lifeTime)
	{
		particle2->constantBufferData.reset = false;
		if (player2->doubleUp == true)
		{
			particle2->constantBufferData.reset = true;
			particle2->particleCounter = 0;
			player2->doubleUp = false;
		}
		particle2->particleCounter += deltaTime * 20;
		if (particle2->particleCounter >= particle2->constantBufferData.lifeTime)
		{
			particle2->constantBufferData.reset = true;
			particle2->particleCounter = 0;
			player2->renderParticles = false;
		}
	}

	levelGenerator->Update(entityHandler, deltaTime, crystalFrenzy);

	if (startMenu->renderMenu == true)
		startMenu->Update(input->GetButtonUpState(), input->GetButtonDownState(), input->GetButtonEnterState());

	if (pauseMenu->renderMenu == true && startMenu->renderMenu == false)
		pauseMenu->Update(input->GetButtonUpState(), input->GetButtonDownState(), input->GetButtonEnterState());

	if (restartMenu->renderMenu == true)
		restartMenu->Update(input->GetButtonUpState(), input->GetButtonDownState(), input->GetButtonEnterState());
	return false;
}

void Application::Render()
{
	d3dHandler->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	d3dHandler->GetDeviceContext()->PSSetConstantBuffers(0, 1, &oneDirectionLightObject.pointerToBufferL);

	if (startMenu->renderMenu == false)
	{
		entityHandler->Render(d3dHandler->GetDeviceContext());
		// Combo - Display text
		comboBarShader->Use(d3dHandler->GetDeviceContext());
		player1->comboDisplayText[0]->Render(d3dHandler->GetDeviceContext(), comboBarShader);
		player1->comboDisplayText[1]->Render(d3dHandler->GetDeviceContext(), comboBarShader);
		player1->comboDisplayText[2]->Render(d3dHandler->GetDeviceContext(), comboBarShader);
		player1->comboDisplayText[3]->Render(d3dHandler->GetDeviceContext(), comboBarShader);

		player2->comboDisplayText[0]->Render(d3dHandler->GetDeviceContext(), comboBarShader);
		player2->comboDisplayText[1]->Render(d3dHandler->GetDeviceContext(), comboBarShader);
		player2->comboDisplayText[2]->Render(d3dHandler->GetDeviceContext(), comboBarShader);
		player2->comboDisplayText[3]->Render(d3dHandler->GetDeviceContext(), comboBarShader);

		// Power Bars
		powerBarShader->Use(d3dHandler->GetDeviceContext());
		player1->powerBar->Render(d3dHandler->GetDeviceContext(), powerBarShader);
		player2->powerBar->Render(d3dHandler->GetDeviceContext(), powerBarShader);

		if (player1->powerBar->IsDead() == true || player2->powerBar->IsDead() == true)
		{
			buttonShader->Use(d3dHandler->GetDeviceContext());
			restartMenu->renderMenu = true;
			restartMenu->Render(d3dHandler->GetDeviceContext());
		}

		comboBarShader->Use(d3dHandler->GetDeviceContext());
		player1->comboBar->Render(d3dHandler->GetDeviceContext(), comboBarShader);
		player2->comboBar->Render(d3dHandler->GetDeviceContext(), comboBarShader);

		// Particles
		if (particle->particleCounter > 0)
		{
			particleShader->Use(d3dHandler->GetDeviceContext());
			particle->Render(d3dHandler->GetDeviceContext());

		}
		if (player2->renderParticles == true)
		{
			particleShader->Use(d3dHandler->GetDeviceContext());
			particle2->Render(d3dHandler->GetDeviceContext());

		}
	}
	// Menu
	if (startMenu->renderMenu == true)
	{
		buttonShader->Use(d3dHandler->GetDeviceContext());
		startMenu->Render(d3dHandler->GetDeviceContext());
		
	}

	if (pauseMenu->renderMenu == true && startMenu->renderMenu == false)
	{
		buttonShader->Use(d3dHandler->GetDeviceContext());
		pauseMenu->Render(d3dHandler->GetDeviceContext());
	}

	d3dHandler->EndScene();
}