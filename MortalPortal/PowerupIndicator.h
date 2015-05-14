#pragma once

#include "Entity.h"
#include "Shader.h"

class PowerupIndicator : public Entity
{

private:
	DirectX::XMFLOAT3 playerPos;

public:
	PowerupIndicator(Geometry* geometry, Material* material, Shader* shader,
		DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 velocity = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 angleVelocity = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 acceleration = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 rotation = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 scale = DirectX::XMFLOAT3(1, 1, 1));

	~PowerupIndicator();
	void Update(float deltaTime);
	void UpdatePosition(DirectX::XMFLOAT3);



};