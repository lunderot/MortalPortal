#pragma once
#include "Entity.h"

class Particle :
	public Entity
{
protected:
	ID3D11UnorderedAccessView* particleUAV;
public:

	Particle(unsigned int nrOfParticles,
		ID3D11Device* device,
		DirectX::XMFLOAT2 position = DirectX::XMFLOAT2(0, 0),
		DirectX::XMFLOAT2 velocity = DirectX::XMFLOAT2(0, 0),
		DirectX::XMFLOAT2 acceleration = DirectX::XMFLOAT2(0, 0));

	~Particle();

};