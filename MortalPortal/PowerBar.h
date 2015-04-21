#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Shader.h"

class PowerBar
{

protected:
	ID3D11Buffer* vertexBuffer;

private:
	DirectX::XMFLOAT2 point[4];

public:

	PowerBar(ID3D11Device* device);
	~PowerBar();
	void setPosition(DirectX::XMFLOAT2 point[4]);

	const DirectX::XMFLOAT2* getPosition();

	ID3D11Buffer* getVertexBuffer();

	void Render(ID3D11DeviceContext* deviceContext, Shader* shader);
};