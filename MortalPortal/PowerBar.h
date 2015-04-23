#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Shader.h"

struct PosColor
{
	DirectX::XMFLOAT2 pos[4];
	DirectX::XMFLOAT2 color;
};

class PowerBar
{
protected:
	ID3D11Buffer* vertexBuffer;
	float barSpeed;

	DirectX::XMFLOAT2 maxMinValue;

	PosColor posColor;

public:

	PowerBar(ID3D11Device* device);
	~PowerBar();

	void SetBarSpeed(float speed);
	void SetPosition(DirectX::XMFLOAT2 point[4]);
	void SetColor(DirectX::XMFLOAT2 color);
	void SetMaxMinValue(DirectX::XMFLOAT2 value);

	const float GetBarSpeed();
	const DirectX::XMFLOAT2* GetPosition();
	const DirectX::XMFLOAT2 GetMaxMinValue();
	ID3D11Buffer* GetVertexBuffer();

	void Update(float deltaTime);
	void Render(ID3D11DeviceContext* deviceContext, Shader* shader);
};