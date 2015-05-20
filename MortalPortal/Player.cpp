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
	) : Entity(geometry, material, shader, position, velocity, angleVelocity, acceleration, rotation, scale)

{ 
	this->switchMaterial = switchMaterial;
	
	powerBar = new PowerBar(device);

	previousButtonState = false;
	colorState = color1;
	colors[0] = color1;
	colors[1] = color2;
	renderParticles = false;
	doubleUp = false;

	// Power ups
	inverControlTimer = 0.0f;
	slowDownAccelerationTimer = 0.0f;
	bonusComboTimer = 0.0f;
	immortalPortalTimer = 0.0f;
	crystalFrenzy = false;
	// Power Ups - Display
	invertControlerDisplay = false;
	slowDownAccelerationDisplay = false;
	bonusComboDisplay = false;

	score = 0;
	comboScore = 0;
	maxComboScore = 999;
	speedBoost = 1.0f;
}


Player::~Player()
{
	delete powerBar;
}

bool Player::GetInvertControl()
{
	return inverControlTimer > 0.0f;
}

bool Player::GetSlowDownAcceleration()
{
	return slowDownAccelerationTimer > 0.0f;
}

bool Player::GetBonusCombo()
{
	return bonusComboTimer > 0.0f;
}

bool Player::GetImmortalPortal()
{
	return immortalPortalTimer > 0.0f;
}

bool Player::GetCrystalFrenzy()
{
	return crystalFrenzy;
}

void Player::SetInvertControl(float powerUp_InvertControl)
{
	this->inverControlTimer = powerUp_InvertControl;
}

void Player::setSlowDownAcceleration(float powerUp_SlowdownAcceleration)
{
	this->slowDownAccelerationTimer = powerUp_SlowdownAcceleration;
}

void Player::setBonusCombo(float powerUp_BonusCombo)
{
	this->bonusComboTimer = powerUp_BonusCombo;
}

void Player::setImmortalPortal(float powerUp_ImmortalPortal)
{
	this->immortalPortalTimer = powerUp_ImmortalPortal;
}

void Player::setCrystalFrenzy(bool powerUp_CrystalFrenzy)
{
	this->crystalFrenzy = powerUp_CrystalFrenzy;
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

void Player::ReactToControl(float timeSinceStart, DirectX::XMFLOAT2 dir, bool invertControl, bool slowDown)
{
	//std::cout << fmod(timeSinceStart, 20.0f) << std::endl;

	if (fmod(timeSinceStart, 5.0f) < 0.001f)
	{
		std::cout << "Player Boost!!! :D" << std::endl;
		speedBoost += 0.1f;
	}

	float slowDownAcc = 5.0f;
	float notSlowDownAcc = 35.0f;
	// Player 1 - control
	// PowerUp - Invert Control - effect on | Player2
	if (invertControl)
	{
		dir.x *= -0.5f * speedBoost;
		dir.y *= -0.5f * speedBoost;
	}

	// PowerUp - Slow Down Acceleration - effect on | Player2
	if (slowDown)
	{
		dir.x *= slowDownAcc * speedBoost;
		dir.y *= slowDownAcc * speedBoost;
	}
	else
	{
		dir.x *= notSlowDownAcc * speedBoost;
		dir.y *= notSlowDownAcc * speedBoost;
	}
	SetAcceleration(DirectX::XMFLOAT3(dir.x, dir.y, 0.0f));
}

void Player::Update(float deltaTime)
{
	Entity::Update(deltaTime);
	DirectX::XMFLOAT3 length;
	DirectX::XMStoreFloat3(&length, DirectX::XMVector3Length(DirectX::XMLoadFloat3(&velocity)));

	if (length.x > 50.0f * speedBoost)
	{
		DirectX::XMVECTOR NormalDir;
		NormalDir = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&velocity));
		NormalDir = DirectX::XMVectorScale(NormalDir, 50.0f * speedBoost);
		XMStoreFloat3(&length, NormalDir);
		velocity.x = length.x;
		velocity.y = length.y;
	}

	for (std::vector<CollisionSphere>::iterator Sphere = geometry->GetCollision()->spheres.begin(); Sphere != geometry->GetCollision()->spheres.end(); Sphere++)
	{
		DirectX::XMVECTOR rotationQuat = DirectX::XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&rotation));
		DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

		DirectX::XMMATRIX model = DirectX::XMMatrixRotationQuaternion(rotationQuat);
		model = DirectX::XMMatrixMultiply(model, DirectX::XMMatrixTranslationFromVector(XMLoadFloat3(&Sphere->position)));
		model = scaleMatrix * model;

		DirectX::XMFLOAT3 posF3;
		DirectX::XMStoreFloat3(&posF3, DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&Sphere->position), model));

		if (position.x + posF3.x - Sphere->radius < -30)
		{
			position.x = -30 - posF3.x + Sphere->radius;
			velocity.x = 0;
		}
		else if (position.x + posF3.x + Sphere->radius > 30)
		{
			position.x = 30 - posF3.x - Sphere->radius;
			velocity.x = 0;
		}

		if (position.y + posF3.y - Sphere->radius < -17)
		{
			position.y = -17 - posF3.y + Sphere->radius;
			velocity.y = 0;
		}
		else if (position.y + posF3.y + Sphere->radius > 17)
		{
			position.y = 17 - posF3.y - Sphere->radius;
			velocity.y = 0;
		}
	}

	slowDownAccelerationTimer -= deltaTime;
	inverControlTimer -= deltaTime;
	bonusComboTimer -= deltaTime;
	immortalPortalTimer -= deltaTime;
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

void Player::AddPower(unsigned int bonusPower)
{
	powerBar->AddPower(bonusPower);
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
	position = DirectX::XMFLOAT3(-15, 0, 0);
	velocity = DirectX::XMFLOAT3(0, 0, 0);
	angleVelocity = DirectX::XMFLOAT3(0, 0, 0);
	acceleration = DirectX::XMFLOAT3(0, 0, 0);
	this->powerBar->Reset();
	this->alive = true;

	if (HasColor(Color::RED))
		this->SetPosition(DirectX::XMFLOAT3(-15, 5, 0));
	else
		this->SetPosition(DirectX::XMFLOAT3(-15, -5, 0));

	

	this->colorState = colors[0];
	this->previousButtonState = false;

	inverControlTimer = 0;
	slowDownAccelerationTimer = 0;
	bonusComboTimer = 0;
	immortalPortalTimer = 0;

	SetInvertControl(0.0f);
	setSlowDownAcceleration(0.0f);
	setBonusCombo(0.0f);
	setImmortalPortal(0.0f);

	score = 0;
	comboScore = 0;
}

void Player::RemoveCombo()
{
	comboScore = 0;
}

void Player::RemovePower()
{
	powerBar->RemovePower();
}

void Player::AddScore(int amount)
{
	score += amount;
	if (score < 0)
	{
		score = 0;
	}
}

int Player::GetScore() const
{
	return score;
}

void Player::AddCombo(unsigned int comboChange)
{
	comboScore += comboChange;
	if (comboScore < 0)
	{
		comboScore = 0;
	}
	if (comboScore > maxComboScore)
	{
		comboScore = maxComboScore;
	}
}
int Player::GetCombo()
{
	return comboScore;
}
