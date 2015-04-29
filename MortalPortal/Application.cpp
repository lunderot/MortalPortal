#include "Application.h"

using namespace DirectX;

Application::Application(bool fullscreen, bool showCursor, int screenWidth, int screenHeight, LPCWSTR windowTitle)
	: System(fullscreen, showCursor, screenWidth, screenHeight, windowTitle)
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

	//Create Audio instance
	audioHandler = new AudioHandler();
	pirate = new Audio(audioHandler, L"assets/audio/pirate.wav");
	pirate->loadAudio();
	//pirate->playAudio();

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
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "assets/Comet.bin", "lambert2"),
		shader);

	//Create player and add it to entity handler
	player1 = new Player(d3dHandler->GetDevice(),
		assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/Player1.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "assets/Player1.bin", "Portal1"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "assets/Player1.bin", "Portal2"),
		playerShader, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(40, 0, 30));
	entityHandler->Add(player1);

	player2 = new Player(d3dHandler->GetDevice(),
		assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/Player2.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "assets/Player2.bin", "Portal1"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "assets/Player2.bin", "Portal2"),
		playerShader, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(40, 0, 30));
	entityHandler->Add(player2);
	
	// Create Combo-bar
	player1->comboBar->setMaterial(new Material*[]{
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "2ggr.dds"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "4ggr.dds")});

	player2->comboBar->setMaterial(new Material*[]{
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "2ggr.dds"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "4ggr.dds")});
	/*player1->comboBar->setMaterial(new Material*[]{
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "assets/Player1.bin", "Portal1"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "assets/Player1.bin", "Portal1")});

	player2->comboBar->setMaterial(new Material*[]{
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "assets/Player2.bin", "Portal1"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "assets/Player2.bin", "Portal1")});*/

	// Create Combo-Display text
	for (unsigned int i = 0; i < 2; i++)
	{
		player1->comboDisplayText[i]->setMaterial(new Material*[]{
			assetHandler->GetMaterial(d3dHandler->GetDevice(), "combo.dds"),
			assetHandler->GetMaterial(d3dHandler->GetDevice(), "zero.dds"),
			assetHandler->GetMaterial(d3dHandler->GetDevice(), "one.dds")/*,
			assetHandler->GetMaterial(d3dHandler->GetDevice(), "two.dds"),
			assetHandler->GetMaterial(d3dHandler->GetDevice(), "three.dds"),
			assetHandler->GetMaterial(d3dHandler->GetDevice(), "four.dds"),
			assetHandler->GetMaterial(d3dHandler->GetDevice(), "five.dds"),
			assetHandler->GetMaterial(d3dHandler->GetDevice(), "six.dds"),
			assetHandler->GetMaterial(d3dHandler->GetDevice(), "seven.dds"),
			assetHandler->GetMaterial(d3dHandler->GetDevice(), "eight.dds"),
			assetHandler->GetMaterial(d3dHandler->GetDevice(), "nine.dds")*/});
	}

	// Particles testing area
	particle = new Particle(10, d3dHandler->GetDevice());

	// Create Background
	entityHandler->Add(
		new Background(

		//assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/test.bin"),
		assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/BackgroundPlane.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "face.dds"),
		playerShader, XMFLOAT3(0, 0, 10), XMFLOAT3(10, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(46, 26, 1)

			////assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/test.bin"),
			//assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/BackgroundPlane.bin"),
			//assetHandler->GetMaterial(d3dHandler->GetDevice(), "assets/Player2.bin", "Portal1"),
			//playerShader, XMFLOAT3(0, 0, 10), XMFLOAT3(1, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(20, 20, 20)

		)
		);



	//// create background
	entityHandler->Add(
		new Background(
		//assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/test.bin"),
		assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/BackgroundPlane.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "face.dds"),
		playerShader, XMFLOAT3(-91.5, 0, 10), XMFLOAT3(10, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(46, 26, 1)
		)
		);

	//entityHandler->Add(
	//	new Background(
	//		//assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/test.bin"),
	//		assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/BackgroundPlane.bin"),
	//		assetHandler->GetMaterial(d3dHandler->GetDevice(), "assets/textures/face.dds"),
	//		playerShader, XMFLOAT3(0, 0, 10), XMFLOAT3(10, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(46, 46, 1)
	//	)
	//);



	//// create background
	//entityHandler->Add(
	//	new Background(
	//	//assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/test.bin"),
	//	assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/BackgroundPlane.bin"),
	//	assetHandler->GetMaterial(d3dHandler->GetDevice(), "assets/textures/face.dds"),
	//	playerShader, XMFLOAT3(-91, 0, 10), XMFLOAT3(10, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(46, 46, 1)
	//	)
	//	);

	player2->powerBar->SetColor(DirectX::XMFLOAT2(1.0f, 1.0f));
	DirectX::XMFLOAT2 player2BarPos[4];
	player2BarPos[0] = DirectX::XMFLOAT2(-0.1f, -0.9f);
	player2BarPos[1] = DirectX::XMFLOAT2(-0.1f, -1.0f);
	player2BarPos[2] = DirectX::XMFLOAT2(-0.7f, -0.9f);
	player2BarPos[3] = DirectX::XMFLOAT2(-0.7f, -1.0f);
	player2->powerBar->SetPosition(player2BarPos);

	// Combo bars, player1 & player2
	// Player 1
	DirectX::XMFLOAT2 player1Pos[4];
	DirectX::XMFLOAT2 player1UV[4];
	player1Pos[0] = DirectX::XMFLOAT2(0.15f, 1.0f); // l�ngst upp - h�ger
	player1Pos[1] = DirectX::XMFLOAT2(0.15f, 0.8f); // l�ngst ner - h�ger
	player1Pos[2] = DirectX::XMFLOAT2(0.0f, 1.0f); // l�ngst upp - v�nster
	player1Pos[3] = DirectX::XMFLOAT2(0.0f, 0.8f); // l�ngst ner - v�nster
	player1UV[0] = DirectX::XMFLOAT2(1.0f, 0.0f);
	player1UV[2] = DirectX::XMFLOAT2(0.0f, 0.0f);
	player1UV[1] = DirectX::XMFLOAT2(1.0f, 1.0f);
	player1UV[3] = DirectX::XMFLOAT2(0.0f, 1.0f);
	player1->comboBar->SetPosition(player1Pos);
	player1->comboBar->SetUV(player1UV);
	
	// Player 2
	DirectX::XMFLOAT2 player2Pos[4];
	DirectX::XMFLOAT2 player2UV[4];
	player2Pos[0] = DirectX::XMFLOAT2(0.15f, -0.8f); // l�ngst upp - h�ger
	player2Pos[1] = DirectX::XMFLOAT2(0.15f, -1.0f); // l�ngst ner - h�ger
	player2Pos[2] = DirectX::XMFLOAT2(0.0f, -0.8f); // l�ngst upp - v�nster
	player2Pos[3] = DirectX::XMFLOAT2(0.0f, -1.0f); // l�ngst ner - v�nster	
	player2UV[0] = DirectX::XMFLOAT2(1.0f, 0.0f);
	player2UV[1] = DirectX::XMFLOAT2(1.0f, 1.0f);
	player2UV[2] = DirectX::XMFLOAT2(0.0f, 0.0f);
	player2UV[3] = DirectX::XMFLOAT2(0.0f, 1.0f);
	player2->comboBar->SetPosition(player2Pos);
	player2->comboBar->SetUV(player2UV);

	// Combo-Display text, player1 & player2
	// Player 1 - "COMBO" text
	player1Pos[0] = DirectX::XMFLOAT2(-0.1f, 0.90f); // l�ngst upp - h�ger
	player1Pos[1] = DirectX::XMFLOAT2(-0.1f, 0.8f); // l�ngst ner - h�ger
	player1Pos[2] = DirectX::XMFLOAT2(-0.3f, 0.90f); // l�ngst upp - v�nster
	player1Pos[3] = DirectX::XMFLOAT2(-0.3f, 0.8f); // l�ngst ner - v�nster
	player1->comboDisplayText[0]->SetPosition(player1Pos);
	player1->comboDisplayText[0]->SetUV(player1UV);
	player1->comboDisplayText[0]->SetComboText(true);
	// player 1 - "NUMBERS" text | FIRST
	player1Pos[0] = DirectX::XMFLOAT2(-0.3f, 0.90f); // l�ngst upp - h�ger
	player1Pos[1] = DirectX::XMFLOAT2(-0.3f, 0.8f); // l�ngst ner - h�ger
	player1Pos[2] = DirectX::XMFLOAT2(-0.35f, 0.90f); // l�ngst upp - v�nster
	player1Pos[3] = DirectX::XMFLOAT2(-0.35f, 0.8f); // l�ngst ner - v�nster
	player1->comboDisplayText[1]->SetPosition(player1Pos);
	player1->comboDisplayText[1]->SetUV(player1UV);
	player1->comboDisplayText[1]->SetComboText(false);


	// Menu
	menu = new Menu(d3dHandler->GetDevice());
	menu->AddButton(new StartButton(
		DirectX::XMFLOAT2(0, 0.4f),
		DirectX::XMFLOAT2(0.1f, 0.1f),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "start.dds")));

	menu->AddButton(new OptionsButton(
		DirectX::XMFLOAT2(0, 0.0f),
		DirectX::XMFLOAT2(0.1f, 0.1f),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "options.dds")));

	menu->AddButton(new QuitButton(
		DirectX::XMFLOAT2(0, -0.4f),
		DirectX::XMFLOAT2(0.1f, 0.1f),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "quit.dds")));

	// Skip the shitty menu
	menu->renderMenu = false;

	// Game Over
	Points gameOverRec;
	gameOverRec.pos[0] = DirectX::XMFLOAT2(-0.5f, -0.5f);
	gameOverRec.pos[1] = DirectX::XMFLOAT2(-0.5f, 0.5f);
	gameOverRec.pos[2] = DirectX::XMFLOAT2(0.5f, -0.5f);
	gameOverRec.pos[3] = DirectX::XMFLOAT2(0.5f, 0.5f);

	gameOverRec.uv[0] = DirectX::XMFLOAT2(0.5f, 0.5f);
	gameOverRec.uv[1] = DirectX::XMFLOAT2(0.5f, 0.5f);
	gameOverRec.uv[2] = DirectX::XMFLOAT2(0.5f, 0.5f);
	gameOverRec.uv[3] = DirectX::XMFLOAT2(0.5f, 0.5f);

	gameOver = new GameOver(gameOverRec, d3dHandler->GetDevice());
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

	delete audioHandler;
	delete pirate;

	delete levelGenerator;

	delete particle;
	delete gameOver;
	delete menu;

}

bool Application::Update(float deltaTime)
{
	menu->CheckIfToPause(input->GetButtonStartState());
	if (menu->renderMenu == true)
	{
		menu->Update(input->GetButtonUpState(), input->GetButtonDownState(), input->GetButtonEnterState());
		deltaTime = 0;
	}
	XMFLOAT2 dir = input->GetDirection(player1Test);
	dir.x *= 10;
	dir.y *= 10;
	player1->SetAcceleration(XMFLOAT3(dir.x, dir.y, 0.0f));
	player1->ReactToInput(input->GetButtonState());

	XMFLOAT2 dir2 = input2->GetDirection(player2Test);
	dir2.x *= 5;
	dir2.y *= 5;
	player2->SetAcceleration(XMFLOAT3(dir2.x, dir2.y, 0.0f));
	player2->ReactToInput(input2->GetButtonState());
	
	player1->powerBar->Update(deltaTime, d3dHandler->GetDeviceContext());
	player2->powerBar->Update(deltaTime, d3dHandler->GetDeviceContext());

	player1->comboBar->Update(deltaTime);
	player2->comboBar->Update(deltaTime);

	player1->comboDisplayText[0]->Update(deltaTime);
	player1->comboDisplayText[1]->Update(deltaTime);

	entityHandler->Update(deltaTime);
	particle->UpdatePosition(player1->GetPosition());
	particle->UpdateDeltaTime(deltaTime);
	levelGenerator->Update(entityHandler, deltaTime);

	menu->Update(input->GetButtonUpState(), input->GetButtonDownState(), input->GetButtonEnterState());


	return false;
}

void Application::Render()
{
	d3dHandler->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	if (menu->renderMenu == false)
	{
		entityHandler->Render(d3dHandler->GetDeviceContext());
		// Combo - Display text
		comboBarShader->Use(d3dHandler->GetDeviceContext());
		player1->comboDisplayText[0]->Render(d3dHandler->GetDeviceContext(), comboBarShader);
		player1->comboDisplayText[1]->Render(d3dHandler->GetDeviceContext(), comboBarShader);

		// Power Bars
		powerBarShader->Use(d3dHandler->GetDeviceContext());
		player1->powerBar->Render(d3dHandler->GetDeviceContext(), powerBarShader);
		player2->powerBar->Render(d3dHandler->GetDeviceContext(), powerBarShader);

		if (player1->powerBar->IsDead() == true)
			gameOver->RenderText(d3dHandler->GetDeviceContext());

		comboBarShader->Use(d3dHandler->GetDeviceContext());
		player1->comboBar->Render(d3dHandler->GetDeviceContext(), comboBarShader);
		player2->comboBar->Render(d3dHandler->GetDeviceContext(), comboBarShader);

		// Particles
		particleShader->Use(d3dHandler->GetDeviceContext());
		particle->Render(d3dHandler->GetDeviceContext(), particleShader, particleShader->GetComputeShader());
	}
	// Menu
	if (menu->renderMenu == true)
	{
		buttonShader->Use(d3dHandler->GetDeviceContext());
		menu->Render(d3dHandler->GetDeviceContext());
		
	}

	d3dHandler->EndScene();
}