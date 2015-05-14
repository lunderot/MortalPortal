#pragma once

#include <DirectXMath.h>
#include <d3d11.h>


class LightL
{
private:
	//enum type {eDirectional, ePoint} lightType;
	DirectX::XMFLOAT3 position;
	bool isDirectional;
	//DirectX::XMFLOAT3 ambient;
	DirectX::XMFLOAT3 diffuse;
	//DirectX::XMFLOAT3 specular;
	//float specularPow;

	struct LightInfo
	{
		DirectX::XMFLOAT3 Position;
		int isDirectional;
		//DirectX::XMFLOAT3 Ambient;
		//float pad1;
		DirectX::XMFLOAT3 Diffuse;
		float pad2;
		//DirectX::XMFLOAT3 Specular;
		//float pad3;
	} directionLInfo;
protected:

public:
		LightL();
	LightL(
		ID3D11Device* device,
		DirectX::XMFLOAT3 position,
		bool isDirectional, 
		DirectX::XMFLOAT3 diffuse);
	~LightL();
	
	void CreateLight(ID3D11Device* device);
	void CreateLight(
		ID3D11Device* device, 
		DirectX::XMFLOAT3 position,
		DirectX::XMFLOAT3 ambient,
		DirectX::XMFLOAT3 diffuse, 
		DirectX::XMFLOAT3 specular, 
		float specularPow);

	ID3D11Buffer* pointerToBufferL = nullptr;
};