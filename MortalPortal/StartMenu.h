#pragma once
#include "StartButton.h"
#include "OptionsButton.h"
#include "QuitButton.h"
#include "Button.h"
#include <d3d11.h>
#include <map>
#include <vector>
#include <string>

class StartMenu
{
protected:
	unsigned int currentSelect;
	std::vector <Button*> buttons;
	ID3D11Buffer* buttonVertexBuffer; 
	ID3D11Buffer* constantBuffer;
	ID3D11ShaderResourceView* SRV;
	Geometry* buttonGeometry;

	unsigned int check;

	DirectX::XMVECTOR scalingOrigin;
	DirectX::XMVECTOR scaling;
	DirectX::XMVECTOR rotationOrigin;
	DirectX::XMVECTOR translation;

public:
	ButtonScale buttonScale;
	StartMenu(ID3D11Device* device);
	~StartMenu();
	bool renderMenu;

	Geometry* GetButtonGeometry();

	void Update(bool gamepadUp, bool gamepadDown, bool aButton);
	void Render(ID3D11DeviceContext* deviceContext);
	void UpdateConstantBuffer(ID3D11DeviceContext* deviceContext, ButtonScale* buffer);

	void AddButton(Button* button);


};