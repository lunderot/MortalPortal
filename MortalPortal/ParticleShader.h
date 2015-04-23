#pragma once
#include "Shader.h"

class ParticleShader :
	public Shader
{
protected:
	ID3D11ComputeShader*	computeShader;
public:
	ParticleShader(LPCWSTR computeShaderFilename, LPCWSTR geometryShaderPath, ID3D11Device* device, LPWSTR vertexShaderPath, LPWSTR pixelShaderPath, unsigned int screenWidth, unsigned int screenHeight, float screenNear, float screenFar);
	~ParticleShader();

	void CreateShaders(ID3D11Device* device, LPCWSTR computeShaderFilename, LPCWSTR geometryShaderFilename);

	ID3D11ComputeShader* GetComputeShader();
	void Use(ID3D11DeviceContext* deviceContext);

};