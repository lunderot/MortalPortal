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
		DirectX::XMFLOAT2 position = DirectX::XMFLOAT2(0, 0),
		DirectX::XMFLOAT2 velocity = DirectX::XMFLOAT2(0, 0),
		DirectX::XMFLOAT2 acceleration = DirectX::XMFLOAT2(0, 0));

	~Particle();
	void SetNrOfParticles(unsigned int nr);

	ID3D11UnorderedAccessView* getUAV();
	unsigned int GetNrOfParticles();

	void Render(ID3D11DeviceContext* deviceContext, Shader* shader, ID3D11ComputeShader* computeShader);

};