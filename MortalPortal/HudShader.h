#pragma once
#include "Shader.h"
class HudShader :
	public Shader
{
public:
	HudShader(ID3D11Device* device, LPWSTR vertexShaderPath, LPWSTR pixelShaderPath, unsigned int screenWidth, unsigned int screenHeight, float screenNear, float screenFar);
	~HudShader();
};

