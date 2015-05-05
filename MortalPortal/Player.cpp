#include "Player.h"

#include <iostream>

Player::Player(ID3D11Device* device, Geometry* geometry, Material* material, Material* switchMaterial, Shader* shader,
	Color color1, Color color2,
	DirectX::XMFLOAT3 position,
	DirectX::XMFLOAT3 velocity,
	DirectX::XMFLOAT3 angleVelocity,
	DirectX::XMFLOAT3 acceleration,
	DirectX::XMFLOAT3 rotation,
	DirectX::XMFLOAT3 scale
	) : Entity(geometry, material, shader, position, velocity, acceleration, rotation, scale)

{ 
	this->switchMaterial = switchMaterial;
	
	powerBar = new PowerBar(device);
	comboBar = new ComboBar(device, material);
	comboDisplayText[0] = new ComboDisplayText(device, material);
	comboDisplayText[1] = new ComboDisplayText(device, material);
	comboDisplayText[2] = new ComboDisplayText(device, material);
	comboDisplayText[3] = new ComboDisplayText(device, material);
	
	playerNumber = 0;
	comboCounter = 0;
	comboCounterChange_10 = 0;
	comboCounterChange_100 = 0;
	comboMax = false;
	previousButtonState = false;
	colorState = color1;
	colors[0] = color1;
	colors[1] = color2;

	//power ups
	inverControlTimer = 0.0f;
}


Player::~Player()
{
	delete powerBar;
	delete comboBar;
	delete comboDisplayText[0];
	delete comboDisplayText[1];
	delete comboDisplayText[2];
	delete comboDisplayText[3];
}

bool Player::getInvertControl()
{
	return inverControlTimer > 0.0f;
}

void Player::setInvertControl(float powerUp_invertControl)
{
	this->inverControlTimer = powerUp_invertControl;
}

void Player::ReactToInput(bool currentButtonState, AudioMaster &aMaster)
{
	if (currentButtonState != previousButtonState)
	{
		if (currentButtonState)
		{
			if (colorState == colors[0])
			{
				colorState = colors[1];
				aMaster.playSample("boing");
			}
			else
			{
				colorState = colors[0];
				aMaster.playSample("boing");
			}
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

	inverControlTimer -= deltaTime;
}

Material* Player::GetMaterial() const
{
	if (colorState == colors[0])
	{
		return material;
	}
	else
	{
		return switchMaterial;
	}
}

void Player::SetPlayerNumber(unsigned int playerNr)
{
	this->playerNumber = playerNr;
}

unsigned int Player::GetPlayerNumber() const
{
	return this->playerNumber;
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

Color Player::GetColor() const
{
	return colorState;
}
void Player::SetColor(Color color)
{
	colorState = color;
}

bool Player::HasColor(Color color) const
{
	for (int i = 0; i < 2; i++)
	{
		if (colors[i] == color)
		{
			return true;
		}
	}
	return false;
}

void Player::Reset()
{
	this->powerBar->Reset();
	this->alive = true;
	this->SetPosition(DirectX::XMFLOAT3(0, 0, 0));
	this->colorState = colors[0];
	this->comboCounter = 0;
	this->comboCounterChange_10 = 0;
	this->comboCounterChange_100 = 0;
	this->comboMax = false;
	this->previousButtonState = false;

	comboDisplayText[0]->RemoveCombo();
	comboDisplayText[1]->RemoveCombo();
	comboDisplayText[2]->RemoveCombo();
	comboBar->RemoveCombo();
}

void Player::RemoveCombo()
{
	comboBar->RemoveCombo();
}

void Player::AddComboText()
{
	comboCounter++;
	comboCounterChange_10++;
	comboCounterChange_100++;

	std::cout << "ComboCounter: " << comboCounter << std::endl;
	std::cout << "ComboCounterChange: " << comboCounter << std::endl;
	
	if (comboCounter == 999)
	{
		comboMax = true;
	}

	if (comboMax != true)
	{
		comboDisplayText[1]->AddCombo();
		if (comboCounterChange_10 == 10)
		{
			comboDisplayText[2]->AddCombo();
			comboCounterChange_10 = 0;
		}
		if (comboCounterChange_100 == 100)
		{
			comboDisplayText[3]->AddCombo();
			comboCounterChange_100 = 0;
		}
	}
}

void Player::RemoveComboText()
{
	comboMax = false;
	comboCounter = 0;
	comboCounterChange_10 = 0;
	comboCounterChange_100 = 0;
	comboDisplayText[1]->RemoveCombo();
	comboDisplayText[2]->RemoveCombo();
	comboDisplayText[3]->RemoveCombo();
}
