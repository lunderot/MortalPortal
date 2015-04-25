#pragma once
#include "Shader.h"
#include <d3d11.h>

class MapItemShader : public Shader
{
protected:
	ID3D11SamplerState* sampleState;

public:
	MapItemShader(ID3D11Device* device, LPWSTR vertexShaderPath, LPWSTR pixelShaderPath, unsigned int screenWidth, unsigned int screenHeight, float screenNear, float screenFar);
	~MapItemShader();

	void Use(ID3D11DeviceContext* deviceContext);
};
