#include "Application.h"

Application::Application(bool fullscreen, bool showCursor, int screenWidth, int screenHeight, LPCWSTR windowTitle)
			: System(fullscreen, showCursor, screenWidth, screenHeight, windowTitle)
{
	float screenDepth = 1000.0f;
	float screenNear = 0.1f;
	d3dHandler = new D3DHandler(screenWidth, screenHeight, hwnd, fullscreen, screenDepth, screenNear);
}

Application::~Application()
{
	delete d3dHandler;
}

bool Application::Update(float deltaTime)
{

	return false;
}

void Application::Render()
{
	d3dHandler->BeginScene(1.0f, 0.0f, 0.0f, 1.0f);

	d3dHandler->EndScene();
}