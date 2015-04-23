#include "Particles.h"

Particle::Particle(unsigned int nrOfParticles,
	ID3D11Device* device,
	DirectX::XMFLOAT3 position,
	DirectX::XMFLOAT3 velocity,
	DirectX::XMFLOAT3 acceleration) : Entity(nullptr, position, velocity, acceleration)
{


	HRESULT hr;
	this->nrOfParticles = nrOfParticles;
	ID3D11Buffer* vertexBuffer = nullptr;

	D3D11_BUFFER_DESC partbufferDesc;
	ZeroMemory(&partbufferDesc, sizeof(partbufferDesc));
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
	uavDesc.Buffer.NumElements = nrOfParticles * 5;

	hr = device->CreateUnorderedAccessView(vertexBuffer, &uavDesc, &particleUAV);
	geometry = new Geometry(vertexBuffer, 0);

	// Constant Buffer
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(constantBufferData);

	hr = device->CreateBuffer(&bufferDesc, NULL, &constantBuffer);
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create constant buffer in Particles");
	}
	// Ett stycke h�rd kod
	constantBufferData.maxRange = 10;
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

void Particle::Render(ID3D11DeviceContext* deviceContext, Shader* shader, ID3D11ComputeShader* computeShader)
{
	ID3D11Buffer* vertexBuffer = geometry->GetVertexBuffer();
	UINT stride = sizeof(float) * 5;
	UINT offset = 0;

	deviceContext->UpdateSubresource(constantBuffer, 0, NULL, &constantBufferData, 0, 0);
	deviceContext->CSSetConstantBuffers(0, 1, &constantBuffer);
	ID3D11UnorderedAccessView* pUAV[] = { particleUAV };
	deviceContext->CSSetUnorderedAccessViews(0, 1, pUAV, NULL);
	deviceContext->CSSetShader(computeShader, nullptr, 0);

	deviceContext->Dispatch(floor(nrOfParticles / 1024) + 1, 1, 1);
	pUAV[0] = NULL;
	deviceContext->CSSetUnorderedAccessViews(0, 1, pUAV, NULL);
	deviceContext->CSSetShader(nullptr, nullptr, 0);
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	deviceContext->Draw(nrOfParticles, 0);
}

Particle::~Particle()
{
	if (particleUAV)
		particleUAV->Release();
	delete geometry;
}