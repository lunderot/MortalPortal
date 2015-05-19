#pragma once
#include "Shader.h"
class OrthoHudShader :
	public Shader
{
public:
	OrthoHudShader(ID3D11Device* device, LPWSTR vertexShaderPath, LPWSTR pixelShaderPath, unsigned int screenWidth, unsigned int screenHeight, float screenNear, float screenFar);
	~OrthoHudShader();
};

