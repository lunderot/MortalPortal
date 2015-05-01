#pragma once
#include "StartButton.h"
#include "OptionsButton.h"
#include "QuitButton.h"
#include "Button.h"
#include <d3d11.h>
#include <map>
#include <vector>
#include <string>

class RestartMenu
{
protected:
	unsigned int currentSelect;
	std::vector <Button*> buttons;
	ID3D11Buffer* buttonVertexBuffer;
	ID3D11Buffer* constantBuffer;
	ID3D11ShaderResourceView* SRV;
	Geometry* buttonGeometry;
	ButtonScale buttonScale;

	unsigned int check;

	DirectX::XMVECTOR scalingOrigin;
	DirectX::XMVECTOR scaling;
	DirectX::XMVECTOR rotationOrigin;
	DirectX::XMVECTOR translation;

public:

	RestartMenu(ID3D11Device* device);
	~RestartMenu();
	bool renderMenu;

	Geometry* GetButtonGeometry();

	void Update(bool gamepadUp, bool gamepadDown, bool aButton);
	void Render(ID3D11DeviceContext* deviceContext);
	void UpdateConstantBuffer(ID3D11DeviceContext* deviceContext, ButtonScale* buffer);

	void AddButton(Button* button);


};