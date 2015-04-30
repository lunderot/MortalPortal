#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Shader.h"
#include "Entity.h"

struct ComboDisplayTextPoints
{
	DirectX::XMFLOAT2 pos[4];
	DirectX::XMFLOAT2 uv[4];
};

class ComboDisplayText
{
protected:
	ID3D11Buffer* vertexBuffer;
	ComboDisplayTextPoints comboDTPoints;

public:
	ComboDisplayText(ID3D11Device* device, Material* materialCombo);
	~ComboDisplayText();

	const DirectX::XMFLOAT2* GetPosition();

	void setMaterial(Material* materialCombo[5]);
	void SetPosition(DirectX::XMFLOAT2 point[4]);
	void SetUV(DirectX::XMFLOAT2 UV[4]);
	void AddCombo();
	void RemoveCombo();
	void SetComboText(bool comboTextState);

	void Update(float deltaTime);
	void Render(ID3D11DeviceContext* deviceContext, Shader* shader);
	ID3D11Buffer* GetVertexBuffer();

private:
	ID3D11ShaderResourceView* SRV;
	Material* materialUsing[5];
	bool comboText;
	unsigned int comboNr;
};