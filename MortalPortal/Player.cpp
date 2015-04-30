#include "Player.h"

#include <iostream>

Player::Player(ID3D11Device* device, Geometry* geometry, Material* material, Material* switchMaterial, Shader* shader,
	DirectX::XMFLOAT3 position,
	DirectX::XMFLOAT3 velocity,
	DirectX::XMFLOAT3 acceleration,
	DirectX::XMFLOAT3 rotation,
	DirectX::XMFLOAT3 scale
	) : Entity(geometry, material, shader, position, velocity, acceleration, rotation, scale)

{ 
	powerBar = new PowerBar(device);
	comboBar = new ComboBar(device, material);
	comboDisplayText[0] = new ComboDisplayText(device, material);
	comboDisplayText[1] = new ComboDisplayText(device, material);
	this->switchMaterial = switchMaterial;
	colorState = 0;
	previousButtonState = false;
}


Player::~Player()
{
	delete powerBar;
	delete comboBar;
	delete comboDisplayText[0];
	delete comboDisplayText[1];
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

void Player::Update(float deltaTime)
{
	Entity::Update(deltaTime);

	for (std::vector<CollisionSphere>::iterator Sphere = geometry->GetCollision()->spheres.begin(); Sphere != geometry->GetCollision()->spheres.end(); Sphere++)
	{
		for (int i = 0; i < 4; i++)
		{
			if (position.x + Sphere->position.x - Sphere->radius < -30)
			{
				position.x = -30 - Sphere->position.y + Sphere->radius;
				velocity.x = 0;
			}
			else if (position.x + Sphere->position.x + Sphere->radius > 30)
			{
				position.x = 30 - Sphere->position.y - Sphere->radius;
				velocity.x = 0;
			}

			if (position.y + Sphere->position.y - Sphere->radius < -14)
			{
				position.y = -14 - Sphere->position.y + Sphere->radius;
				velocity.y = 0;
			}
			if (position.y + Sphere->position.y + Sphere->radius > 14)
			{
				position.y = 14 - Sphere->position.y - Sphere->radius;
				velocity.y = 0;
			}
		}
	}

}

Material* Player::GetMaterial() const
{
	if (colorState == 0)
	{
		return material;
	}
	else
	{
		return switchMaterial;
	}
}

void Player::AddPower()
{
	powerBar->AddPower();
}

void Player::RemovePower()
{
	powerBar->RemovePower();
}

void Player::AddCombo()
{
	comboBar->AddCombo();
}

void Player::RemoveCombo()
{
	comboBar->RemoveCombo();
}

void Player::AddComboText()
{
	comboDisplayText[1]->AddCombo();
}

void Player::RemoveComboText()
{
	comboDisplayText[1]->RemoveCombo();
}
