#pragma once
#include <d3d11.h>
#include <vector>
#include <map>
class TextureHandler
{
protected:
	std::map<std::string, ID3D11ShaderResourceView*> texture;

public:
	TextureHandler();
	~TextureHandler();
	ID3D11ShaderResourceView* LoadTexture(std::string name);

};