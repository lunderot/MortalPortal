#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Shader.h"
#include "Button.h"

class RectangleScreen
{
protected:
	ID3D11Buffer* vertexBuffer;
	ID3D11ShaderResourceView* SRV;
	Material* material1;
	Material* material2;
	buttonPoint* rectPoints;
public:
	RectangleScreen(buttonPoint* points, Material* mat1, Material* mat2, ID3D11Device* device);
	~RectangleScreen();
	bool player1Wins;
	bool playerWinsText;

	void Render(ID3D11DeviceContext* deviceContext);
};