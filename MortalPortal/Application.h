#pragma once
#include <iostream>
#include <DirectXMath.h>

#include "System.h"
#include "D3DHandler.h"
#include "Shader.h"
#include "Importer.h"
#include "Input.h"
#include "ControllerInput.h"
#include "KeyboardInput.h"
#include "EntityHandler.h"


class Application : public System
{
private:
	D3DHandler* d3dHandler;
	Shader* testShader;
	Importer testImporter;
	Input* input;

	ID3D11Buffer* testVertexBuffer;

	EntityHandler* entityHandler;
public:
	Application(bool fullscreen, bool showCursor, int screenWidth, int screenHeight, LPCWSTR windowTitle);
	~Application();

	bool Update(float deltaTime); //Returns true on quit
	void Render();
};

