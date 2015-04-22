#include "Particles.h"

Particle::Particle(unsigned int nrOfParticles,
	ID3D11Device* device,
	DirectX::XMFLOAT2 position,
	DirectX::XMFLOAT2 velocity,
	DirectX::XMFLOAT2 acceleration) : Entity(device, position, velocity, acceleration)
{
	HRESULT hr;

	D3D11_BUFFER_DESC partbufferDesc;
	ZeroMemory(&partbufferDesc, sizeof(partbufferDesc));
	memset(&partbufferDesc, 0, sizeof(partbufferDesc));
	partbufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_VERTEX_BUFFER;
	partbufferDesc.Usage = D3D11_USAGE_DEFAULT;
	partbufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT2) * nrOfParticles;
	partbufferDesc.StructureByteStride = sizeof(DirectX::XMFLOAT2);
	partbufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;

	/*D3D11_SUBRESOURCE_DATA datar;
	datar.pSysMem = &partVertices;*/

	hr = device->CreateBuffer(&partbufferDesc, 0, &vertexBuffer);

	//COMPUTE SHADER
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof(uavDesc));
	uavDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
	uavDesc.Buffer.NumElements = sizeof(DirectX::XMFLOAT2) * nrOfParticles / 4;

	hr = device->CreateUnorderedAccessView(vertexBuffer, &uavDesc, &particleUAV);
}

Particle::~Particle()
{
	if (particleUAV)
		particleUAV->Release();
}