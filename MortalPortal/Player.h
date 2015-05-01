#pragma once
#include "Entity.h"
#include "PowerBar.h"
#include "ComboBar.h"
#include "ComboDisplayText.h"

class Player :
	public Entity
{
public:

	Player(ID3D11Device* device, Geometry* geometry, Material* material, Material* switchMaterial, Shader* shader,
		DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 velocity = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 angleVelocity = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 acceleration = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 rotation = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 scale = DirectX::XMFLOAT3(1, 1, 1)
		);
	~Player();

	void ReactToInput(bool currentButtonState);
	void Update(float deltaTime);

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

	void Reset();

private:
	Material* switchMaterial;
	unsigned int colorState;
	unsigned int comboCounter;
	unsigned int comboCounterChange_10;
	unsigned int comboCounterChange_100;
	bool comboMax;
	bool previousButtonState;
};

