#pragma once
#include "Shader.h"

class BackgroundShader : public Shader
{
public:
	BackgroundShader(ID3D11Device* device, LPWSTR vertexShaderPath, LPWSTR pixelShaderPath, unsigned int screenWidth, unsigned int screenHeight, float screenNear, float screenFar);
	~BackgroundShader();

};