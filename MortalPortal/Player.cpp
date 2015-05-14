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
	comboBar = new ComboBar(device, material);
	comboDisplayText[0] = new ComboDisplayText(device, material);
	comboDisplayText[1] = new ComboDisplayText(device, material);
	comboDisplayText[2] = new ComboDisplayText(device, material);
	comboDisplayText[3] = new ComboDisplayText(device, material);
	powerUpDisplayText[0] = new PowerUpDisplayText(device, material);
	powerUpDisplayText[1] = new PowerUpDisplayText(device, material);
	powerUpDisplayText[2] = new PowerUpDisplayText(device, material);
	powerUpDisplayText[3] = new PowerUpDisplayText(device, material);

	comboCounter = 0;
	comboCounterChange_10 = 0;
	comboCounterChange_100 = 0;
	comboMax = false;
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
}


Player::~Player()
{
	delete powerBar;
	delete comboBar;
	delete comboDisplayText[0];
	delete comboDisplayText[1];
	delete comboDisplayText[2];
	delete comboDisplayText[3];
	delete powerUpDisplayText[0];
	delete powerUpDisplayText[1];
	delete powerUpDisplayText[2];
	delete powerUpDisplayText[3];
}

bool Player::getInvertControl()
{
	if (inverControlTimer < 0.0f)
	{
		powerUpDisplayText[0]->RemoveTextDisplay();
	}
	return inverControlTimer > 0.0f;
}

bool Player::getSlowDownAcceleration()
{
	if (slowDownAccelerationTimer < 0.0f)
	{
		powerUpDisplayText[1]->RemoveTextDisplay();
	}
	return slowDownAccelerationTimer > 0.0f;
}

bool Player::getBonusCombo()
{
	if (bonusComboTimer < 0.0f)
	{
		powerUpDisplayText[2]->RemoveTextDisplay();
	}
	return bonusComboTimer > 0.0f;
}

bool Player::getImmortalPortal()
{
	if (immortalPortalTimer < 0.0f)
	{
		powerUpDisplayText[3]->RemoveTextDisplay();
	}
	return immortalPortalTimer > 0.0f;
}

bool Player::getCrystalFrenzy()
{
	return crystalFrenzy;
}

void Player::setInvertControl(float powerUp_InvertControl)
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

void Player::Update(float deltaTime)
{
	Entity::Update(deltaTime);

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

		if (position.y + posF3.y - Sphere->radius < -18)
		{
			position.y = -18 - posF3.y + Sphere->radius;
			velocity.y = 0;
		}
		else if (position.y + posF3.y + Sphere->radius > 18)
		{
			position.y = 18 - posF3.y - Sphere->radius;
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

void Player::AddInvertControlDisplay()
{
	powerUpDisplayText[0]->AddTextDisplay();
}

void Player::RemoveInvertControlDisplay()
{
	invertControlerDisplay = false;
}

void Player::AddSlowDownAccelerationDisplay()
{
	powerUpDisplayText[1]->AddTextDisplay();
}

void Player::RemoveSlowDownAccelerationDisplay()
{
	slowDownAccelerationDisplay = false;
}

void Player::AddBonusComboDisplay()
{
	powerUpDisplayText[2]->AddTextDisplay();
}

void Player::AddImmortalPortalDisplay()
{
	powerUpDisplayText[3]->AddTextDisplay();
}

void Player::AddPower(unsigned int bonusPower)
{
	powerBar->AddPower(bonusPower);
}

void Player::AddCombo(bool bonusComboControl)
{
	if (comboCounterChange_10 >= 9)
	{
		comboBar->AddCombo(bonusComboControl);
	}

	if (bonusComboControl == true)
	{
		comboBar->AddCombo(bonusComboControl);
	}
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
		if (comboCounterChange_10 >= 10)
		{
			comboDisplayText[2]->AddCombo();
			comboCounterChange_10 = 0;
		}
		if (comboCounterChange_100 >= 100)
		{
			comboDisplayText[3]->AddCombo();
			comboCounterChange_100 = 0;
		}
	}
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
	this->SetPosition(DirectX::XMFLOAT3(-15, 0, 0));
	this->colorState = colors[0];
	this->comboCounter = 0;
	this->comboCounterChange_10 = 0;
	this->comboCounterChange_100 = 0;
	this->comboMax = false;
	this->previousButtonState = false;

	inverControlTimer = 0;
	slowDownAccelerationTimer = 0;
	bonusComboTimer = 0;
	immortalPortalTimer = 0;

	setInvertControl(0.0f);
	setSlowDownAcceleration(0.0f);
	setBonusCombo(0.0f);
	setImmortalPortal(0.0f);
	comboDisplayText[0]->RemoveCombo();
	comboDisplayText[1]->RemoveCombo();
	comboDisplayText[2]->RemoveCombo();
	comboBar->RemoveCombo();

	score = 0;
}

void Player::RemoveCombo()
{
	comboBar->RemoveCombo();
}

void Player::RemovePower()
{
	powerBar->RemovePower();
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
