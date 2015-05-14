#pragma once
#include "Shader.h"
class ObjectShader :
	public Shader
{
public:
	ObjectShader(ID3D11Device* device, LPWSTR vertexShaderPath, LPWSTR pixelShaderPath, unsigned int screenWidth, unsigned int screenHeight, float screenNear, float screenFar);
	~ObjectShader();
};

