#pragma once
#include "Entity.h"
#include "Shader.h"

struct ConstantBufferData
{
	UINT maxRange;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 velocity;
	DirectX::XMFLOAT2 acceleration;
};

class Particle :
	public Entity
{
protected:
	ID3D11UnorderedAccessView* particleUAV;
	ID3D11Buffer* constantBuffer;

	ConstantBufferData constantBufferData;
	unsigned int nrOfParticles;
public:

	Particle(unsigned int nrOfParticles,
		ID3D11Device* device,
		DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 velocity = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 acceleration = DirectX::XMFLOAT3(0, 0, 0));

	~Particle();

	ID3D11UnorderedAccessView* getUAV();
	unsigned int GetNrOfParticles();

	void UpdatePosition(DirectX::XMFLOAT3 pos);
	void Render(ID3D11DeviceContext* deviceContext, Shader* shader, ID3D11ComputeShader* computeShader);

};