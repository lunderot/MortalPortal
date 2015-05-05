#pragma once
#include "Entity.h"
#include "PowerBar.h"
#include "ComboBar.h"
#include "ComboDisplayText.h"
#include "AudioMaster.h"

class Player :
	public Entity
{
public:

	Player(ID3D11Device* device, Geometry* geometry, Material* material, Material* switchMaterial, Shader* shader,
		Color color1, Color color2,
		DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 velocity = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 angleVelocity = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 acceleration = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 rotation = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 scale = DirectX::XMFLOAT3(1, 1, 1)
		);
	~Player();

	void ReactToInput(bool currentButtonState, AudioMaster &aMaster);
	void Update(float deltaTime);
	
	unsigned int GetPlayerNumber() const;
	void SetPlayerNumber(unsigned int playerNr);

	Material* GetMaterial() const;
	void AddPower();
	void RemovePower();
	PowerBar* powerBar;

	void AddCombo();
	void RemoveCombo();
	ComboBar* comboBar;

	void AddComboText();
	void RemoveComboText();
	ComboDisplayText* comboDisplayText[4];
	
	Color GetColor() const;
	void SetColor(Color color);
	bool HasColor(Color color) const;
	
	// power ups
	// Invert Control
	bool getInvertControl();
	void setInvertControl(float powerUp_InvertControl);
	// Slow Down Acceleration
	bool getSlowDownAcceleration();
	void setSlowDownAcceleration(float powerUp_SlowDownAcceleration);
	// Crystal Frenzy
	bool getCrystalFrenzy();
	void setCrystalFrenzy(bool powerUp_CrystalFrenzy);

	void Reset();

private:
	// Power ups
	float inverControlTimer;
	float slowDownAccelerationTimer;
	bool crystalFrenzy;

	Material* switchMaterial;
	
	unsigned int comboCounter;
	unsigned int comboCounterChange_10;
	unsigned int comboCounterChange_100;
	bool comboMax;
	bool previousButtonState;
	unsigned int playerNumber;

	Color colorState;
	Color colors[2];
};

