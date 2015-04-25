#include "MapItemShader.h"

MapItemShader::MapItemShader(ID3D11Device* device, LPWSTR vertexShaderPath, LPWSTR pixelShaderPath, unsigned int screenWidth, unsigned int screenHeight, float screenNear, float screenFar) : Shader(device, screenWidth, screenHeight, screenNear, screenFar)
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	CreateMandatoryShaders(device, vertexShaderPath, pixelShaderPath, inputDesc, ARRAYSIZE(inputDesc));

	/*D3D11_SAMPLER_DESC sampleStateDesc;
	ZeroMemory(&sampleStateDesc, sizeof(sampleStateDesc));
	sampleStateDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampleStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampleStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampleStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampleStateDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	HRESULT hr = device->CreateSamplerState(&sampleStateDesc, &sampleState);

	if (FAILED(hr))
		throw std::runtime_error("Failed to CreateSamplerState 'MapItemShader::MapItemShader()'");*/
}

MapItemShader::~MapItemShader()
{
	//sampleState->Release();
}

void MapItemShader::Use(ID3D11DeviceContext* deviceContext)
{
	Shader::Use(deviceContext); 

	deviceContext->PSSetSamplers(0, 1, &sampleState);
}