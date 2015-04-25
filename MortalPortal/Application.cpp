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

	playerShader = new PlayerShader(d3dHandler->GetDevice(), L"assets/shaders/playerVS.hlsl", L"assets/shaders/playerPS.hlsl", screenWidth, screenHeight, screenNear, screenFar);

	mapItemShader = new MapItemShader(d3dHandler->GetDevice(), L"assets/shaders/MapItemVS.hlsl", L"assets/shaders/MapItemPS.hlsl", screenWidth, screenHeight, screenNear, screenFar);

	// Ayu
	backgShader = new BackgroundShader(d3dHandler->GetDevice(), L"assets/shaders/BackgroundVertexShader.hlsl", L"assets/shaders/BackgroundPixelShader.hlsl", screenWidth, screenHeight, screenNear, screenFar);

	particleShader = new ParticleShader(L"assets/shaders/particleCS.hlsl", L"assets/shaders/particleGS.hlsl", d3dHandler->GetDevice(), L"assets/shaders/particleVS.hlsl", L"assets/shaders/particlePS.hlsl", screenWidth, screenHeight, screenNear, screenFar);

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

	levelGenerator->setPlayerOneCrystals(assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/Player1Crystal.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "assets/textures/grass.dds"),
		mapItemShader,
		assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/Player1Crystal.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "assets/textures/snow.dds"),
		mapItemShader);

	levelGenerator->setPlayerTwoCrystals(assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/Player2Crystal.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "assets/textures/grass.dds"),
		mapItemShader,
		assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/Player2Crystal.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "assets/textures/snow.dds"),
		mapItemShader);

	//Add avaible comets for LevelGenerator so choos from
	levelGenerator->addComet(assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/test.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "assets/textures/grass.dds"),
		shader);

	//Create player and add it to entity handler
	player1 = new Player(
		assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/test.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "assets/textures/grass.dds"), 
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "assets/textures/snow.dds"),
		playerShader, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(40, 0, 30));
	entityHandler->Add(player1);

	player2 = new Player(assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/test.bin"),
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "assets/textures/grass.dds"), 
		assetHandler->GetMaterial(d3dHandler->GetDevice(), "assets/textures/snow.dds"),
		playerShader, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(40, 0, 30));
	entityHandler->Add(player2);

	// Particles testing area
	particle = new Particle(10, d3dHandler->GetDevice());

	// Create Background
	background = new Background(d3dHandler->GetDevice());

	// Create Power Bars
	player1Bar = new PowerBar(d3dHandler->GetDevice());
	player2Bar = new PowerBar(d3dHandler->GetDevice());

	player2Bar->SetColor(DirectX::XMFLOAT2(1.0f, 1.0f));
	DirectX::XMFLOAT2 player2BarPos[4];

	player2BarPos[0] = DirectX::XMFLOAT2(0.5f, 1.0f);
	player2BarPos[1] = DirectX::XMFLOAT2(0.5f, 0.9f);
	player2BarPos[2] = DirectX::XMFLOAT2(0.5f, 1.0f);
	player2BarPos[3] = DirectX::XMFLOAT2(0.5f, 0.9f);
	player2Bar->SetPosition(player2BarPos);
	player2Bar->SetMaxMinValue(DirectX::XMFLOAT2(0.7f, 0.5f));


}

Application::~Application()
{
	delete d3dHandler;

	delete shader;
	delete powerBarShader;
	delete particleShader;
	delete playerShader;
	delete backgShader;
	delete mapItemShader;

	delete input;
	delete input2;

	delete entityHandler;
	delete assetHandler;

	delete levelGenerator;

	delete particle;
	delete background;

	delete player1Bar;
	delete player2Bar;

}

bool Application::Update(float deltaTime)
{
	XMFLOAT2 dir = input->GetDirection(player1Test);
	dir.x *= 10;
	dir.y *= 10;
	player1->SetAcceleration(XMFLOAT3(dir.x, dir.y, 0.0f));
	player1->ReactToInput(input->GetButtonState());

	XMFLOAT2 dir2 = input2->GetDirection(player2Test);
	dir2.x *= 10;
	dir2.y *= 10;
	player2->SetAcceleration(XMFLOAT3(dir2.x, dir2.y, 0.0f));
	player2->ReactToInput(input2->GetButtonState());
	

	player1Bar->Update(deltaTime);
	player2Bar->Update(deltaTime);
	entityHandler->Update(deltaTime);
	particle->UpdatePosition(player1->GetPosition());
	levelGenerator->Update(entityHandler, deltaTime);

	return false;
}

void Application::Render()
{
	d3dHandler->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	entityHandler->Render(d3dHandler->GetDeviceContext());

	// Power Bars
	powerBarShader->Use(d3dHandler->GetDeviceContext());
	player1Bar->Render(d3dHandler->GetDeviceContext(), powerBarShader);
	player2Bar->Render(d3dHandler->GetDeviceContext(), powerBarShader);


	// Ayu
	//Avkommentera ifall bakgrunden ska synas (z ej klar)
	//backgShader->Use(d3dHandler->GetDeviceContext());
	//background->Render(d3dHandler->GetDeviceContext(), backgShader);


	// Particles
	particleShader->Use(d3dHandler->GetDeviceContext());
	particle->Render(d3dHandler->GetDeviceContext(), particleShader, particleShader->GetComputeShader());

	d3dHandler->EndScene();
}