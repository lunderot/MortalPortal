#include "TextureHandler.h"

TextureHandler::TextureHandler()
{

}

ID3D11ShaderResourceView* TextureHandler::LoadTexture(std::string name)
{
	if (texture.find(name) != texture.end())
	{
		return texture[name];
	}

	HRESULT hr;

	//Create DDS Texture

	//loadtexture stuff
	ID3D11ShaderResourceView* DDSTexture;
	//texture[name] = DDSTexture;

	return NULL;
}

TextureHandler::~TextureHandler()
{
	for (std::map<std::string, ID3D11ShaderResourceView*>::iterator i = texture.begin(); i != texture.end(); ++i)
	{
		(*i).second->Release();
	}
}