#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Shader.h"
#include "Material.h"

struct Points
{
	DirectX::XMFLOAT2 pos;
	DirectX::XMFLOAT2 uv;
};

class PowerBar
{
protected:
	ID3D11Buffer* vertexBuffer;
	float barSpeed;
	float powerAdd;
	float powerRemove;
	bool dead;
	Material* material;

	DirectX::XMFLOAT2 maxMinValue;

	Points points[4];

public:

	PowerBar(ID3D11Device* device);
	~PowerBar();

	void SetBarSpeed(float speed);
	void SetPosition(DirectX::XMFLOAT2 point[4]);
	void SetMaxMinValue(DirectX::XMFLOAT2 value);
	void AddPower(unsigned int bonusPower);
	void RemovePower();
	void SetMaterial(Material* mat);
	const float GetBarSpeed();
	const bool IsDead();
	const DirectX::XMFLOAT2 GetMaxMinValue();
	ID3D11Buffer* GetVertexBuffer();

	void Update(float deltaTime, ID3D11DeviceContext* deviceContext);
	void Render(ID3D11DeviceContext* deviceContext, Shader* shader);
	void Reset();
};