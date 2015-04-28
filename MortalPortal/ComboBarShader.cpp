#include "ComboBarShader.h"

ComboBarShader::ComboBarShader(ID3D11Device* device, LPWSTR vertexShaderPath, LPWSTR pixelShaderPath, unsigned int screenWidth, unsigned int screenHeight, float screenNear, float screenFar)
	: Shader(device, screenWidth, screenHeight, screenNear, screenFar)
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	CreateMandatoryShaders(device, vertexShaderPath, pixelShaderPath, inputDesc, ARRAYSIZE(inputDesc));
}

ComboBarShader::~ComboBarShader()
{

}