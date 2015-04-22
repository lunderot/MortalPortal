#pragma once
#include "Shader.h"

class ParticleShader :
	public Shader
{
public:
	ParticleShader(LPCWSTR computeShaderFilename,ID3D11Device* device, LPWSTR vertexShaderPath, LPWSTR pixelShaderPath, unsigned int screenWidth, unsigned int screenHeight, float screenNear, float screenFar);
	~ParticleShader();

};