#pragma once
#include "Button.h"
#include "ContinueButton.h"
#include <d3d11.h>
#include <map>
#include <vector>
#include <string>

class PauseMenu
{
protected:
	unsigned int currentSelect;
	std::vector <Button*> buttons;
	ID3D11Buffer* buttonVertexBuffer;
	ID3D11Buffer* constantBuffer;
	ID3D11ShaderResourceView* SRV;
	Geometry* buttonGeometry;

	unsigned int check;
	unsigned int pauseCheck;

	DirectX::XMVECTOR scalingOrigin;
	DirectX::XMVECTOR scaling;
	DirectX::XMVECTOR rotationOrigin;
	DirectX::XMVECTOR translation;

public:

	PauseMenu(ID3D11Device* device);
	~PauseMenu();
	bool renderMenu;
	ButtonScale buttonScale;
	Geometry* GetButtonGeometry();
	void CheckIfToPause(bool gamepadStart);
	void Update(bool gamepadUp, bool gamepadDown, bool aButton);
	void Render(ID3D11DeviceContext* deviceContext);
	void UpdateConstantBuffer(ID3D11DeviceContext* deviceContext, ButtonScale* buffer);

	void AddButton(Button* button);


};