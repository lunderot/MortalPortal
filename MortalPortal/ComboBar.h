#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Shader.h"
#include "Entity.h"

struct ComboPoints
{
	DirectX::XMFLOAT2 pos[4];
	DirectX::XMFLOAT2 uv[4];
};

class ComboBar
{
protected:
	ID3D11Buffer* vertexBuffer;
	ComboPoints comboPoints;

public:
	ComboBar(ID3D11Device* device, Material* materialCombo);
	~ComboBar();

	const DirectX::XMFLOAT2* GetPosition();

	void setMaterial(Material* materialCombo[9]);
	void SetPosition(DirectX::XMFLOAT2 point[4]);
	void SetUV(DirectX::XMFLOAT2 UV[4]);
	void AddCombo();
	void RemoveCombo();
	
	void Update(float deltaTime);
	void Render(ID3D11DeviceContext* deviceContext, Shader* shader);
	ID3D11Buffer* GetVertexBuffer();

private:
	ID3D11ShaderResourceView* SRV;
	Material* materialUsing[9];
	unsigned int changeCombo;
};