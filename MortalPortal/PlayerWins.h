#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Shader.h"
#include "Button.h"

class PlayerWins
{
protected:
	ID3D11Buffer* vertexBuffer;
	ID3D11ShaderResourceView* SRV;
	Material* material1;
	Material* material2;
public:
	PlayerWins(Material* mat1, Material* mat2, ID3D11Device* device);
	~PlayerWins();
	bool player1Wins;

	void RenderText(ID3D11DeviceContext* deviceContext);
};