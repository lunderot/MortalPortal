#pragma once
#include "StartButton.h"
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

	std::vector <Button*> buttons;
	ID3D11Buffer* buttonVertexBuffer; 
	ID3D11Buffer* constantBuffer;
	Geometry* buttonGeometry;
	ButtonScale buttonScale;

	DirectX::XMVECTOR scalingOrigin;
	DirectX::XMVECTOR scaling;
	DirectX::XMVECTOR rotationOrigin;
	DirectX::XMVECTOR translation;

public:
	
	Menu(ID3D11Device* device);
	~Menu();

	Geometry* GetButtonGeometry();
	void Update(float deltaTime);
	void Render(ID3D11DeviceContext* deviceContext);
	void UpdateConstantBuffer(ID3D11DeviceContext* deviceContext, ButtonScale* buffer);

	void AddButton(Button* button);

};