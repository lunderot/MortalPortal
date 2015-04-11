#include "Application.h"


Application::Application(bool fullscreen, bool showCursor, int screenWidth, int screenHeight, LPCWSTR windowTitle)
			: System(fullscreen, showCursor, screenWidth, screenHeight, windowTitle)
{

}


Application::~Application()
{
}

bool Application::Update(float deltaTime)
{

	return false;
}

void Application::Render()
{
	
}
