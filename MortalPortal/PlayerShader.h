#pragma once
#include "Shader.h"
#include <DirectXTex.h>

//mange
struct ConstantBufferPerState
{
	UINT colorState;
	UINT colorState2;
	float pad2;
	float pad3;
};

class PlayerShader :
	public Shader
{
public:
	PlayerShader(ID3D11Device* device, LPWSTR vertexShaderPath, LPWSTR pixelShaderPath, unsigned int screenWidth, unsigned int screenHeight, float screenNear, float screenFar);
	~PlayerShader();

	//mange
	ID3D11Buffer*				constantBufferPerState;
	ConstantBufferPerState		constantBufferPerStateData;
	ID3D11ShaderResourceView*	test;

	DirectX::TexMetadata texMetadata;
	DirectX::ScratchImage image;

	unsigned int colorState = 0;
	void Render(ID3D11DeviceContext* deviceContext, Shader* shader);
};