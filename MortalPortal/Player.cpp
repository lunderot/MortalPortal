#include "Player.h"

#include <iostream>

Player::Player(Geometry* geometry, Material* material, Material* switchMaterial, Shader* shader,
	DirectX::XMFLOAT3 position,
	DirectX::XMFLOAT3 velocity,
	DirectX::XMFLOAT3 acceleration,
	DirectX::XMFLOAT3 rotation
	) : Entity(geometry, material, shader, position, velocity, acceleration, rotation)
{
	this->switchMaterial = switchMaterial;
	colorState = 0;
	previousButtonState = false;
}


Player::~Player()
{

}

void Player::ReactToInput(bool currentButtonState)
{
	if (currentButtonState != previousButtonState)
	{
		if (currentButtonState)
		{
			if (colorState == 0)
			{
				colorState = 1;
			}
			else
				colorState = 0;
		}
	}
	previousButtonState = currentButtonState;
}



Material* Player::GetMaterial() const
{
	if (colorState == 0)
	{
		return material;
	}
	else if (colorState == 1)
	{
		return switchMaterial;
	}
}

