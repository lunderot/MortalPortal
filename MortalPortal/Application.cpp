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

	// Ayu
	backgShader = new BackgroundShader(d3dHandler->GetDevice(), L"assets/shaders/BackgroundVertexShader.hlsl", L"assets/shaders/BackgroundPixelShader.hlsl", screenWidth, screenHeight, screenNear, screenFar);

	particleShader = new ParticleShader(L"assets/shaders/particleCS.hlsl", L"assets/shaders/particleGS.hlsl", d3dHandler->GetDevice(), L"assets/shaders/powerBarVS.hlsl", L"assets/shaders/powerBarPS.hlsl", screenWidth, screenHeight, screenNear, screenFar);
	

	//Setup input
	try
	{
		input = new ControllerInput();
		std::cout << "Using controller input" << std::endl;
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
		std::cout << "Using keyboard input" << std::endl;
		input = new KeyboardInput();
	}

	//Create assetHandler
	assetHandler = new AssetHandler();

	//Import asset
	testImporter.importFile("assets/test.bin");

	//Setup entity handler
	entityHandler = new EntityHandler();

	//Create player and add it to entity handler
	player = new Player(assetHandler->GetGeometry(d3dHandler->GetDevice(), "assets/test.bin"), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(40, 0, 30));
	entityHandler->Add(player);

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

	// Particles testing area
	particle = new Particle(10, d3dHandler->GetDevice());
	particle->SetNrOfParticles(10);
	entityHandler->Add(particle);

	// Create Background
	background = new Background(d3dHandler->GetDevice());

}

Application::~Application()
{
	delete d3dHandler;
	delete shader;
	delete input;
	delete entityHandler;
	delete assetHandler;
	delete powerBarShader;
	delete player1Bar;
	delete player2Bar;
	//delete particle;
	delete particleShader;
}

bool Application::Update(float deltaTime)
{
	XMFLOAT2 dir = input->GetDirection();

	dir.x /= 5;
	dir.y /= 5;


	player->SetAcceleration(XMFLOAT3(dir.x, dir.y, 0.0f));
	//mange
	//player->PlayerColorState(player->colorState);
	player->colorState = input->GetButtonState();
	shader->constantBufferPerStateData.colorState = player->colorState;

	player1Bar->Update(deltaTime);
	player2Bar->Update(deltaTime);
	entityHandler->Update(deltaTime);

	return false;
}

void Application::Render()
{
	d3dHandler->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	shader->Use(d3dHandler->GetDeviceContext());

	entityHandler->Render(d3dHandler->GetDeviceContext(), shader);

	powerBarShader->Use(d3dHandler->GetDeviceContext());
	player1Bar->Render(d3dHandler->GetDeviceContext(), powerBarShader);
	player2Bar->Render(d3dHandler->GetDeviceContext(), powerBarShader);


	// Ayu
	//Avkommentera ifall bakgrunden ska synas (z ej klar)
	//backgShader->Use(d3dHandler->GetDeviceContext());
	//background->Render(d3dHandler->GetDeviceContext(), backgShader);



	particleShader->Use(d3dHandler->GetDeviceContext());
	particle->Render(d3dHandler->GetDeviceContext(), particleShader, particleShader->GetComputeShader());

	d3dHandler->EndScene();
}