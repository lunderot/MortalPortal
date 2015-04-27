#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Shader.h"
#include "Entity.h"

struct PosColorr
{
	DirectX::XMFLOAT2 pos[4];
	DirectX::XMFLOAT2 color;
};

class ComboBar
{
protected:
	ID3D11Buffer* vertexBuffer;
	PosColorr posColor;

public:
	ComboBar(ID3D11Device* device, Material* materialCombo);
	~ComboBar();

	void setMaterial(Material* materialCombo);


	void SetPosition(DirectX::XMFLOAT2 point[4]);
	void SetColor(DirectX::XMFLOAT2 color);
	
	const DirectX::XMFLOAT2* GetPosition();
	ID3D11Buffer* GetVertexBuffer();

	void Update(float deltaTime);
	void Render(ID3D11DeviceContext* deviceContext, Shader* shader);

private:
	Material* materialUsing;
};