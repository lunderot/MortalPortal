#include "Application.h"

using namespace DirectX;

Application::Application(bool fullscreen, bool showCursor, int screenWidth, int screenHeight, LPCWSTR windowTitle)
			: System(fullscreen, showCursor, screenWidth, screenHeight, windowTitle)
{
	float screenFar = 1000.0f;
	float screenNear = 0.1f;
	d3dHandler = new D3DHandler(screenWidth, screenHeight, hwnd, fullscreen, screenFar, screenNear);

	shader = new DefaultShader(d3dHandler->GetDevice(), L"assets/shaders/vs.hlsl", L"assets/shaders/ps.hlsl", screenWidth, screenHeight, screenNear, screenFar);

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

	testImporter.importFile("assets/test.bin");

	entityHandler = new EntityHandler();
	player = new Player(d3dHandler->GetDevice(), &testImporter, 0, XMFLOAT2(0, 0), XMFLOAT2(1, 1));
	entityHandler->Add(player);
}

Application::~Application()
{
	delete d3dHandler;
	delete shader;
	delete input;
	delete entityHandler;

	if (testVertexBuffer)
		testVertexBuffer->Release();
}

bool Application::Update(float deltaTime)
{
	XMFLOAT2 dir = input->GetDirection();
	dir.x *= 10;
	dir.y *= 10;
	player->SetAcceleration(dir);

	entityHandler->Update(deltaTime);

	return false;
}

void Application::Render()
{
	d3dHandler->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	shader->Use(d3dHandler->GetDeviceContext());

	entityHandler->Render(d3dHandler->GetDeviceContext(), shader);

	d3dHandler->EndScene();
}