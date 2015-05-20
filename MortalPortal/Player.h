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
	void ReactToControl(float timeSinceStart, DirectX::XMFLOAT2 dir, bool invertControl, bool slowDown);
	void Update(float deltaTime);

	Material* GetMaterial() const;
	void AddPower(unsigned int bonusPower);
	void RemovePower();
	PowerBar* powerBar;


	void RemoveCombo();
	void RemoveBonus();

	Color GetColor() const;
	void SetColor(Color color);
	bool HasColor(Color color) const;
	bool renderParticles;
	bool doubleUp;
	
	// Power ups
	// Invert Control
	bool GetInvertControl();
	void SetInvertControl(float powerUp_InvertControl);
	// Slow Down Acceleration
	bool GetSlowDownAcceleration();
	void setSlowDownAcceleration(float powerUp_SlowDownAcceleration);
	// Bonus Combo
	bool GetBonusCombo();
	void setBonusCombo(float powerUp_BonusCombo);
	// Immortal Portal
	bool GetImmortalPortal();
	void setImmortalPortal(float powerUp_ImmortalPortal);
	// Crystal Frenzy
	bool GetCrystalFrenzy();
	void setCrystalFrenzy(bool powerUp_CrystalFrenzy);

	void Reset();

	void AddScore(int amount);
	int GetScore() const;

	void AddCombo(unsigned int comboChange);
	int GetCombo();

	void AddBonus(unsigned int bonusChange);
	int GetBonus();

	// Audio control
	bool GetApplauseControl();
	void SetApplauseControl(bool applauseControl);

private:
	// Power ups
	float inverControlTimer;
	float slowDownAccelerationTimer;
	float bonusComboTimer;
	float immortalPortalTimer;
	bool crystalFrenzy;
	// Power ups - Display
	bool invertControlerDisplay;
	bool slowDownAccelerationDisplay;
	bool bonusComboDisplay;

	unsigned int bonusCounter;

	// Audio control
	bool ApplauseControl;

	Material* switchMaterial;

	bool previousButtonState;

	Color colorState;
	Color colors[2];

	int score;
	int comboScore;
	int maxComboScore;

	int bonusScore;
	int maxBonusScore;

	float speedBoost;
};

