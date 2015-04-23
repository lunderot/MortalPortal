#pragma once
#include "Entity.h"
#include "Shader.h"

class Particle :
	public Entity
{
protected:
	ID3D11UnorderedAccessView* particleUAV;
	unsigned int nrOfParticles;
public:

	Particle(unsigned int nrOfParticles,
		ID3D11Device* device,
		DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 velocity = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 acceleration = DirectX::XMFLOAT3(0, 0, 0));

	~Particle();
	void SetNrOfParticles(unsigned int nr);

	ID3D11UnorderedAccessView* getUAV();
	unsigned int GetNrOfParticles();

	void Render(ID3D11DeviceContext* deviceContext, Shader* shader, ID3D11ComputeShader* computeShader);

};