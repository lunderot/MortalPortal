#pragma once
#include "System.h"
#include "D3DHandler.h"
#include "Shader.h"
#include "Importer.h"

class Application : public System
{
private:
	D3DHandler* d3dHandler;
	Shader* testShader;
	Importer testImporter;

	ID3D11Buffer* testVertexBuffer;
public:
	Application(bool fullscreen, bool showCursor, int screenWidth, int screenHeight, LPCWSTR windowTitle);
	~Application();

	bool Update(float deltaTime); //Returns true on quit
	void Render();
};

