#include "Player.h"

#include <iostream>

Player::Player(Geometry* geometry, Material* material, Shader* shader,
	DirectX::XMFLOAT3 position,
	DirectX::XMFLOAT3 velocity,
	DirectX::XMFLOAT3 acceleration,
	DirectX::XMFLOAT3 rotation
	) : Entity(geometry, material, shader, position, velocity, acceleration, rotation)
{

}


Player::~Player()
{

}
