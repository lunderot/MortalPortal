#pragma once
#include "Entity.h"
#include "Shader.h"
#include <vector>

struct ConstantBufferData
{
	float lifeTime;
	DirectX::XMFLOAT3 position;
	float deltaTime;
	bool reset;
	DirectX::XMFLOAT2 pad;
};

struct Particles
{
	DirectX::XMFLOAT3 pos;
	unsigned int type;
	DirectX::XMFLOAT2 acceleration;
	DirectX::XMFLOAT2 velocity;
	float lifeTime;

};

class Particle :
	public Entity
{
protected:
	ID3D11UnorderedAccessView* particleUAV;
	ID3D11Buffer* constantBuffer;
	ID3D11ShaderResourceView* SRV;
	Material* material;

	unsigned int nrOfParticles;
public:

	Particle(unsigned int type,
		const unsigned int nrOfParticles,
		Material* material,
		ID3D11Device* device,
		DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 velocity = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 angleVelocity = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 acceleration = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 scale = DirectX::XMFLOAT3(1, 1, 1));

	~Particle();

	ID3D11UnorderedAccessView* getUAV();
	unsigned int GetNrOfParticles();
	int particleCounter;
	std::vector<Particles> particle;
	ConstantBufferData constantBufferData;

	void UpdatePosition(DirectX::XMFLOAT3 pos);
	void UpdateParticle(float deltaTime, ID3D11DeviceContext* deviceContext, ID3D11ComputeShader* computeShader);
	void SetLifeTime(float time);
	void Render(ID3D11DeviceContext* deviceContext);
	void Reset();
	void UpdateConstantBuffer(ID3D11DeviceContext* deviceContext);

};