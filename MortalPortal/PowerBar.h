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
	float powerRemaining;
	bool dead;
	bool endGame;
	Material* material;

	DirectX::XMFLOAT2 maxMinValue;

	Points points[4];

public:

	PowerBar(ID3D11Device* device);
	~PowerBar();

	void SetBarSpeed(float speed);
	void SetPosition(DirectX::XMFLOAT2 point[4]);
	DirectX::XMFLOAT2 GetCurrentMaxPosition();
	void SetMaxMinValue(DirectX::XMFLOAT2 value);
	void AddPower(unsigned int bonusPower);
	void RemovePower(float remove);
	void EndGame();
	void SetMaterial(Material* mat);
	const float GetBarSpeed();
	const bool IsDead();
	const DirectX::XMFLOAT2 GetMaxMinValue();
	ID3D11Buffer* GetVertexBuffer();
	bool removingPower;
	bool addingPower;
	float powerRemovalValue;
	float powerAddValue;
	float GetPowerRemaining();

	void Update(float deltaTime, ID3D11DeviceContext* deviceContext);
	void Render(ID3D11DeviceContext* deviceContext, Shader* shader);
	void Reset();
};