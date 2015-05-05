#include "Light.h"


void LightL::CreateLight(ID3D11Device* device)
{
	directionLInfo.Position = DirectX::XMFLOAT3(0.5f, 0.5f, -1.0f);
	directionLInfo.Ambient = DirectX::XMFLOAT3(0.4f, 0.4f, 0.4f);
	directionLInfo.Diffuse = DirectX::XMFLOAT3(0.4f, 0.4f, 0.4f);
	directionLInfo.Specular = DirectX::XMFLOAT3(0.8f, 0.8f, 0.8f);

	//Buffer
	D3D11_BUFFER_DESC value;
	ZeroMemory(&value, sizeof(D3D11_BUFFER_DESC));
	value.ByteWidth = sizeof(directionLInfo);
	value.Usage = D3D11_USAGE_DEFAULT;
	value.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	value.CPUAccessFlags = 0;
	value.MiscFlags = 0;
	value.StructureByteStride = 0;

	//--- pekare till var info finns dvs till point light buffern
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &directionLInfo;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	//--- Create buffer
	HRESULT hr = device->CreateBuffer(&value, &data, &pointerToBufferL);
}

void LightL::CreateLight(ID3D11Device* device, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 ambient, DirectX::XMFLOAT3 diffuse, DirectX::XMFLOAT3 specular, float specularPow)
{
	this->position = position;
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->specularPow = specularPow;

	directionLInfo.Position = DirectX::XMFLOAT3( 0.5f, 0.5f, -1.0f );
	directionLInfo.Ambient = DirectX::XMFLOAT3( 0.39f, 0.65f, 0.92f );
	directionLInfo.Diffuse = DirectX::XMFLOAT3( 0.9f, 0.9f, 0.9f );
	directionLInfo.Specular = DirectX::XMFLOAT3(0.8f, 0.8f, 0.8f);

	//Buffer
	D3D11_BUFFER_DESC value;
	ZeroMemory(&value, sizeof(D3D11_BUFFER_DESC));
	value.ByteWidth = sizeof(directionLInfo);
	value.Usage = D3D11_USAGE_DEFAULT;
	value.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	value.CPUAccessFlags = 0;
	value.MiscFlags = 0;
	value.StructureByteStride = 0;

	//--- pekare till var info finns dvs till point light buffern
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &directionLInfo;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	//--- Create buffer
	HRESULT hr = device->CreateBuffer(&value, &data, &pointerToBufferL);
}

LightL::LightL()
{

}

LightL::LightL(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 ambient, DirectX::XMFLOAT3 diffuse, DirectX::XMFLOAT3 specular, float specularPow)
{
	this->position = position;
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->specularPow = specularPow;
}

LightL::~LightL()
{

}

