#pragma once
#include "Shader.h"

class ComboBarShader :
	public Shader
{
public:
	ComboBarShader(ID3D11Device* device, LPWSTR vertexShaderPath, LPWSTR pixelShaderPath, unsigned int screenWidth, unsigned int screenHeight, float screenNear, float screenFar);
	~ComboBarShader();

};