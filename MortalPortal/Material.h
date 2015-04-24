#pragma once
#include <d3d11.h>

class Material
{
private:
	ID3D11ShaderResourceView* texture;
public:
	Material(ID3D11ShaderResourceView* texture);
	~Material();

	ID3D11ShaderResourceView* GetTexture() const;
};

