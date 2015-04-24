#include "Material.h"


Material::Material(ID3D11ShaderResourceView* texture)
{
	this->texture = texture;
}


Material::~Material()
{
}


ID3D11ShaderResourceView* Material::GetTexture() const
{
	return texture;
}