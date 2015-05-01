#include "TextureHandler.h"

TextureHandler::TextureHandler()
{

}

ID3D11ShaderResourceView* TextureHandler::LoadTexture(std::string name, ID3D11Device* device)
{
	if (texture.find(name) != texture.end())
	{
		return texture[name];
	}

	HRESULT hr;

	//Create DDS Texture
	ID3D11ShaderResourceView* DDSTexture;
	DirectX::TexMetadata texMetadata;
	DirectX::ScratchImage image;
	std::wstring stemp = std::wstring(name.begin(), name.end());
	std::wstring tmp = L"assets/textures/" + std::wstring(stemp);
	LPCWSTR filePathAndNAme = tmp.c_str();
	hr = DirectX::LoadFromDDSFile(filePathAndNAme, DirectX::DDS_FLAGS_NONE, &texMetadata, image);
	if (FAILED(hr))
		throw std::runtime_error("Failed to LoadFromDDSFile in TextureHandler: " + name);

	hr = DirectX::CreateShaderResourceView(device, image.GetImages(), image.GetImageCount(), texMetadata, &DDSTexture);
	if (FAILED(hr))
		throw std::runtime_error("Failed to CreateShaderResourceView in TextureHandler");

	//Map the name & ShaderResourceView
	texture[name] = DDSTexture;

	return DDSTexture;
}

TextureHandler::~TextureHandler()
{
	for (std::map<std::string , ID3D11ShaderResourceView*>::iterator i = texture.begin(); i != texture.end(); ++i)
	{
		(*i).second->Release();
	}
}