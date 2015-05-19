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
	Material* material1;
	Material* material2;

	unsigned int nrOfParticles;
public:

	Particle(unsigned int type,
		const unsigned int nrOfParticles,
		Material* material1,
		Material* material2,
		ID3D11Device* device,
		DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 velocity = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 angleVelocity = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 acceleration = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 scale = DirectX::XMFLOAT3(1, 1, 1));

	~Particle();
	ID3D11ShaderResourceView* SRV2;
	ID3D11UnorderedAccessView* GetUAV();
	unsigned int GetNrOfParticles();
	float particleCounter;
	std::vector<Particles> particle;
	ConstantBufferData constantBufferData;
	bool changeTexture;
	bool renderPortalEngine;
	bool powerBar;

	void UpdatePosition(DirectX::XMFLOAT3 pos);
	void UpdateParticle(float deltaTime, ID3D11DeviceContext* deviceContext, ID3D11ComputeShader* computeShader);
	void SetLifeTime(float time);
	void Render(ID3D11DeviceContext* deviceContext);
	void Reset();
	void UpdateConstantBuffer(ID3D11DeviceContext* deviceContext);
	void SetMaterial(Material* material);
	void UpdateColor(bool renderParticles, const Color color, Particle* explosionParticles, std::vector<Material*> materials);

};