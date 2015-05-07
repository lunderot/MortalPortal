#include "Material.h"


Material::Material(ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* normal_map, float normal_depth, DirectX::XMFLOAT3 specular, float specular_factor, DirectX::XMFLOAT3 ambient, DirectX::XMFLOAT3 diffuse, DirectX::XMFLOAT3 transparency_color, DirectX::XMFLOAT3 incandescence)
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

