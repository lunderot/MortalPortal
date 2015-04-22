#pragma once
#include "Shader.h"

class PowerBarShader :
	public Shader
{
public:
	PowerBarShader(ID3D11Device* device, LPWSTR vertexShaderPath, LPWSTR pixelShaderPath, unsigned int screenWidth, unsigned int screenHeight, float screenNear, float screenFar);
	~PowerBarShader();

};