#pragma once
#include "System.h"
#include "D3DHandler.h"

class Application : public System
{
private:
	D3DHandler* d3dHandler;
public:
	Application(bool fullscreen, bool showCursor, int screenWidth, int screenHeight, LPCWSTR windowTitle);
	~Application();

	bool Update(float deltaTime); //Returns true on quit
	void Render();
};

