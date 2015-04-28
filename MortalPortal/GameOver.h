#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Shader.h"

struct Points
{
	DirectX::XMFLOAT2 pos[4];
	DirectX::XMFLOAT2 uv[4];
};
class GameOver
{
protected:
	ID3D11Buffer* vertexBuffer;
	Points points;
public:
	GameOver(Points points, ID3D11Device* device);
	~GameOver();

	void RenderText(ID3D11DeviceContext* deviceContext);
};