#pragma once
#include <d3d11.h>
#include <vector>
#include <map>
#include <DirectXTex.h>
class TextureHandler
{
protected:
	std::map<LPCWSTR, ID3D11ShaderResourceView*> texture;

public:
	TextureHandler();
	~TextureHandler();
	ID3D11ShaderResourceView* LoadTexture(LPCWSTR name, ID3D11Device* device);

};