#pragma once
#include "Entity.h"

class Player :
	public Entity
{
public:

	Player(Geometry* geometry, Material* material, Material* switchMaterial, Shader* shader,
		DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 velocity = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 acceleration = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 rotation = DirectX::XMFLOAT3(0, 0, 0)
		);
	~Player();

	void ReactToInput(bool currentButtonState);

	Material* GetMaterial() const;

private:
	unsigned int colorState;
	Material* switchMaterial;
	bool previousButtonState;
};

