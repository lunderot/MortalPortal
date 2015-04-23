#include "BackgroundShader.h"

BackgroundShader::BackgroundShader(ID3D11Device* device, LPWSTR vertexShaderPath, LPWSTR pixelShaderPath, 
								   unsigned int screenWidth, unsigned int screenHeight, float screenNear, float screenFar)
	: Shader(device, screenWidth, screenHeight, screenNear, screenFar)

{
	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	CreateMandatoryShaders(device, vertexShaderPath, pixelShaderPath, inputDesc, ARRAYSIZE(inputDesc));
}



BackgroundShader:: ~BackgroundShader()
{

}