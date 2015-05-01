#include "Particles.h"

Particle::Particle(unsigned int nrOfParticles,
	ID3D11Device* device,
	DirectX::XMFLOAT3 position,
	DirectX::XMFLOAT3 velocity,
	DirectX::XMFLOAT3 angleVelocity,
	DirectX::XMFLOAT3 acceleration, DirectX::XMFLOAT3 scale) : Entity(nullptr, nullptr, nullptr, position, velocity, angleVelocity, acceleration, scale)
{


	HRESULT hr;
	this->nrOfParticles = nrOfParticles;
	ID3D11Buffer* vertexBuffer = nullptr;

	D3D11_BUFFER_DESC partbufferDesc;
	memset(&partbufferDesc, 0, sizeof(partbufferDesc));
	partbufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_VERTEX_BUFFER;
	partbufferDesc.Usage = D3D11_USAGE_DEFAULT;
	partbufferDesc.ByteWidth = sizeof(float) * 5 * nrOfParticles;
	partbufferDesc.StructureByteStride = sizeof(float) * 5;
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
	uavDesc.Buffer.NumElements = nrOfParticles * sizeof(float) * 5 / 4;

	hr = device->CreateUnorderedAccessView(vertexBuffer, &uavDesc, &particleUAV);
	geometry = new Geometry(vertexBuffer, 0, nullptr);

	// Constant Buffer
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(constantBufferData);
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	hr = device->CreateBuffer(&bufferDesc, NULL, &constantBuffer);
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create constant buffer in Particles");
	}
	// Ett stycke hård kod
	constantBufferData.lifeTime = 20;
}

unsigned int Particle::GetNrOfParticles()
{
	return nrOfParticles;
}
ID3D11UnorderedAccessView* Particle::getUAV()
{
	return particleUAV;
}

void Particle::UpdatePosition(DirectX::XMFLOAT3 position)
{
	constantBufferData.position = position;
}

void Particle::UpdateDeltaTime(float deltaTime)
{
	constantBufferData.deltaTime = deltaTime;
}

void Particle::SetLifeTime(float time)
{
	constantBufferData.lifeTime = time;
}

void Particle::Render(ID3D11DeviceContext* deviceContext, Shader* shader, ID3D11ComputeShader* computeShader)
{
	ID3D11Buffer* vertexBuffer = geometry->GetVertexBuffer();
	UINT stride = sizeof(float) * 5;
	UINT offset = 0;

	deviceContext->UpdateSubresource(constantBuffer, 0, NULL, &constantBufferData, 0, 0);
	ID3D11UnorderedAccessView* pUAV[] = { particleUAV };
	deviceContext->CSSetUnorderedAccessViews(0, 1, pUAV, 0);
	deviceContext->CSSetShader(computeShader, nullptr, 0);
	deviceContext->CSSetConstantBuffers(0, 1, &constantBuffer);

	deviceContext->Dispatch((nrOfParticles / 32) + 1, 1, 1);
	pUAV[0] = nullptr;
	deviceContext->CSSetUnorderedAccessViews(0, 1, pUAV, 0);
	deviceContext->CSSetShader(nullptr, nullptr, 0);
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	//deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	deviceContext->Draw(nrOfParticles, 0);
}

Particle::~Particle()
{
	if (particleUAV)
		particleUAV->Release();
	if (geometry->GetVertexBuffer())
		geometry->GetVertexBuffer()->Release();
	delete geometry;
}