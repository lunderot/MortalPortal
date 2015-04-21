#include "Application.h"

using namespace DirectX;

Application::Application(bool fullscreen, bool showCursor, int screenWidth, int screenHeight, LPCWSTR windowTitle)
			: System(fullscreen, showCursor, screenWidth, screenHeight, windowTitle)
{
	float screenFar = 1000.0f;
	float screenNear = 0.1f;
	d3dHandler = new D3DHandler(screenWidth, screenHeight, hwnd, fullscreen, screenFar, screenNear);

	testShader = new Shader(d3dHandler->GetDevice(), screenWidth, screenHeight, screenNear, screenFar);

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

	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	testShader->CreateMandatoryShaders(d3dHandler->GetDevice(), L"assets/shaders/vs.hlsl", L"assets/shaders/ps.hlsl", inputDesc, ARRAYSIZE(inputDesc));

	entityHandler = new EntityHandler();
	player = new Player(d3dHandler->GetDevice(), &testImporter, 0, XMFLOAT2(0, 0), XMFLOAT2(1, 1));
	entityHandler->Add(player);
}

Application::~Application()
{
	delete d3dHandler;
	delete testShader;
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
	testShader->Use(d3dHandler->GetDeviceContext());

	entityHandler->Render(d3dHandler->GetDeviceContext(), testShader);

	d3dHandler->EndScene();
}