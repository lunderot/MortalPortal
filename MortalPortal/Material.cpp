#include "Material.h"


Material::Material(ID3D11Device* device, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* normal_map, float normal_depth, DirectX::XMFLOAT3 specular, float specular_factor, DirectX::XMFLOAT3 ambient, DirectX::XMFLOAT3 diffuse, DirectX::XMFLOAT3 transparency_color, DirectX::XMFLOAT3 incandescence)
{
	this->texture = texture;
	this->normalMap = normal_map;

	this->normalDepth = normal_depth;
	this->specular = specular;
	this->specularFactor = specular_factor;
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->transparencyColor = transparency_color;
	this->incandescence = incandescence;

	materialInfo.Specular = this->specular;
	materialInfo.Specular_factor = this->specularFactor;
	materialInfo.Ambient = this->ambient;
	materialInfo.Diffuse = this->diffuse;
	materialInfo.Transparency_Color = this->transparencyColor;
	materialInfo.Normal_Depth = this->normalDepth;

	//Buffer
	D3D11_BUFFER_DESC value;
	ZeroMemory(&value, sizeof(D3D11_BUFFER_DESC));
	value.ByteWidth = sizeof(materialInfo);
	value.Usage = D3D11_USAGE_DEFAULT;
	value.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	value.CPUAccessFlags = 0;
	value.MiscFlags = 0;
	value.StructureByteStride = 0;

	//--- pekare till var info finns dvs till point material buffern
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &materialInfo;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	//--- Create buffer
	HRESULT hr = device->CreateBuffer(&value, &data, &pointerToBufferM);
}


Material::~Material()
{

}

ID3D11ShaderResourceView* Material::GetTexture() const
{
	return this->texture;
}

ID3D11ShaderResourceView* Material::GetNormalMap() const
{
	return this->normalMap;
}

float Material::GetNormalDepth()
{
	return this->normalDepth;
}

DirectX::XMFLOAT3 Material::GetSpecular()
{
	return this->specular;
}

float Material::GetSpecularFactor()
{
	return this->specularFactor;
}

DirectX::XMFLOAT3 Material::GetAmbient()
{
	return this->ambient;
}

DirectX::XMFLOAT3 Material::GetDiffuse()
{
	return this->diffuse;
}

DirectX::XMFLOAT3 Material::GetTransparencyColor()
{
	return this->transparencyColor;
}

DirectX::XMFLOAT3 Material::GetIncandescence()
{
	return this->incandescence;
}

