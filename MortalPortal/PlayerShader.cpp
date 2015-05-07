#include "PlayerShader.h"

PlayerShader::PlayerShader(ID3D11Device* device, LPWSTR vertexShaderPath, LPWSTR pixelShaderPath, unsigned int screenWidth, unsigned int screenHeight, float screenNear, float screenFar)
	: Shader(device, screenWidth, screenHeight, screenNear, screenFar)
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	//constantBufferPerState = nullptr;
	//D3D11_BUFFER_DESC colorStateBufferDesc;
	//ZeroMemory(&colorStateBufferDesc, sizeof(D3D11_BUFFER_DESC));
	//colorStateBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	//colorStateBufferDesc.ByteWidth = sizeof(constantBufferPerStateData);
	//colorStateBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//colorStateBufferDesc.CPUAccessFlags = 0;
	//colorStateBufferDesc.MiscFlags = 0;
	//device->CreateBuffer(&colorStateBufferDesc, NULL, &constantBufferPerState);
	//HRESULT hr = DirectX::CreateDDSTextureFromFile(device, L"grass.dds", nullptr, &test);
	//HRESULT hr = DirectX::LoadFromDDSFile(L"grass.dds", DirectX::DDS_FLAGS_NONE, &texMetadata, image);
	//hr = DirectX::CreateShaderResourceView(device, image.GetImages(), image.GetImageCount(), texMetadata, &test);
	CreateMandatoryShaders(device, vertexShaderPath, pixelShaderPath, inputDesc, ARRAYSIZE(inputDesc));

}

void PlayerShader::Render(ID3D11DeviceContext* deviceContext, Shader* shader)
{
	//deviceContext->UpdateSubresource(constantBufferPerState, 0, NULL, &constantBufferPerStateData, 0, 0);
	//deviceContext->PSSetConstantBuffers(0, 1, &constantBufferPerState);

	//deviceContext->PSSetShaderResources(0, 1, &test);

	//// n
	//deviceContext->PSSetShaderResources(1, 1, &normalm);


}

PlayerShader::~PlayerShader()
{

}