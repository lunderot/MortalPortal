#pragma once
#include "Shader.h"
class DefaultShader :
	public Shader
{
public:
	DefaultShader(ID3D11Device* device, LPWSTR vertexShaderPath, LPWSTR pixelShaderPath, unsigned int screenWidth, unsigned int screenHeight, float screenNear, float screenFar);
	~DefaultShader();
};

