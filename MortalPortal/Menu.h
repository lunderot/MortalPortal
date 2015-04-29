#pragma once
#include "StartButton.h"
#include "QuitButton.h"
#include "Button.h"
#include <d3d11.h>
#include <map>
#include <vector>
#include <string>

struct buttonPoint
{
	DirectX::XMFLOAT2 position;
	DirectX::XMFLOAT2 UV;
};

struct ButtonScale
{
	DirectX::XMMATRIX scale;
};

class Menu
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
	
	Menu(ID3D11Device* device);
	~Menu();
	bool renderMenu;

	Geometry* GetButtonGeometry();
	void CheckIfToPause();
	void Update();
	void Render(ID3D11DeviceContext* deviceContext);
	void UpdateConstantBuffer(ID3D11DeviceContext* deviceContext, ButtonScale* buffer);

	void AddButton(Button* button);

};