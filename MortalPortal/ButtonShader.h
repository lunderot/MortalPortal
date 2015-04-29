#pragma once
#include "Shader.h"

class ButtonShader :
	public Shader
{
public:
	ButtonShader(ID3D11Device* device, LPWSTR vertexShaderPath, LPWSTR pixelShaderPath, unsigned int screenWidth, unsigned int screenHeight, float screenNear, float screenFar);
	~ButtonShader();

};