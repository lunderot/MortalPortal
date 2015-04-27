#pragma once
#include "Entity.h"
#include "PowerBar.h"

class Player :
	public Entity
{
public:

	Player(ID3D11Device* device, Geometry* geometry, Material* material, Material* switchMaterial, Shader* shader,
		DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 velocity = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 acceleration = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 rotation = DirectX::XMFLOAT3(0, 0, 0)
		);
	~Player();

	void ReactToInput(bool currentButtonState);

	Material* GetMaterial() const;
	void AddPower();
	void RemovePower();
	PowerBar* powerBar;

private:
	unsigned int colorState;
	Material* switchMaterial;
	bool previousButtonState;
};

