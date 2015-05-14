#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

class Material
{
private:
	ID3D11ShaderResourceView* texture;
	ID3D11ShaderResourceView* normalMap;

	float normalDepth;
	DirectX::XMFLOAT3 specular;
	float specularFactor;
	DirectX::XMFLOAT3 ambient;
	DirectX::XMFLOAT3 diffuse;
	DirectX::XMFLOAT3 transparencyColor;
	DirectX::XMFLOAT3 incandescence;

	struct MaterialInformation
	{	
		DirectX::XMFLOAT3 Specular;
		float Specular_factor;
		DirectX::XMFLOAT3 Ambient;
		float pad1;
		DirectX::XMFLOAT3 Diffuse;
		float pad2;
		DirectX::XMFLOAT3 Transparency_Color;
		float Normal_Depth;
	} 
	materialInfo;

public:
	Material(
		ID3D11Device* device,
		ID3D11ShaderResourceView* texture = nullptr, 
		ID3D11ShaderResourceView* normal_map = nullptr,
		float normal_depth = 0.0f,
		DirectX::XMFLOAT3 specular = DirectX::XMFLOAT3(0, 0, 0),
		float specular_factor = 0.0f,
		DirectX::XMFLOAT3 ambient = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 diffuse = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 transparency_color = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 incandescence = DirectX::XMFLOAT3(0, 0, 0));
	~Material();

	ID3D11ShaderResourceView* GetTexture() const;
	ID3D11ShaderResourceView* GetNormalMap() const;
	float GetNormalDepth();
	DirectX::XMFLOAT3 GetSpecular();
	float GetSpecularFactor();
	DirectX::XMFLOAT3 GetAmbient();
	DirectX::XMFLOAT3 GetDiffuse();
	DirectX::XMFLOAT3 GetTransparencyColor();
	DirectX::XMFLOAT3 GetIncandescence();

	ID3D11Buffer* pointerToBufferM = nullptr;
};

