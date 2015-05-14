#include "Particles.h"

Particle::Particle(unsigned int type,
	const unsigned int nrOfParticles, 
	Material* material1,
	Material* material2,
	ID3D11Device* device,
	DirectX::XMFLOAT3 position,
	DirectX::XMFLOAT3 velocity,
	DirectX::XMFLOAT3 angleVelocity,
	DirectX::XMFLOAT3 acceleration, DirectX::XMFLOAT3 scale) : Entity(nullptr, nullptr, nullptr, position, velocity, angleVelocity, acceleration, scale)
{
	SRV = nullptr;
	this->material1 = material1;
	this->material2 = material2;
	changeTexture = false;

	// Crystal Pick-up
	if (type == 1)
	{
		for (unsigned int i = 0; i < nrOfParticles; i++)
		{
			Particles p;
			p.type = type;
			p.lifeTime = 0;// (float)(rand()) / (float)(RAND_MAX / 2.0f);
			p.pos.x = 0;
			p.pos.y = 0;
			p.pos.z = 0;
			float r = (float)(rand()) / (float)(RAND_MAX / DirectX::XM_PIDIV4) + DirectX::XM_PI - DirectX::XM_PIDIV4 * 0.5;
			p.velocity.x = (rand() % 100 + 50) * cos(r);
			p.velocity.y = (rand() % 100 + 50) * sin(r);
			p.acceleration.x = 0;
			p.acceleration.y = 0;
			particle.push_back(p);
		}
	}

	// Power Bar
	if (type == 2)
	{
		for (unsigned int i = 0; i < nrOfParticles; i++)
		{
			Particles p;
			p.type = type;
			p.lifeTime = rand() % 100 - 10;
			p.pos.x = 20;
			p.pos.y = 20;
			p.pos.z = 20;

			p.velocity.x = rand() % 10;
			p.velocity.y = rand() % 10 - 5;
			p.acceleration.x = 0;
			p.acceleration.y = 0;
			particle.push_back(p);
		}
	}

	// Background particles
	if (type == 3)
	{
		for (unsigned int i = 0; i < nrOfParticles; i++)
		{
			Particles p;
			p.type = type;
			p.lifeTime = (float)(rand()) / (float)(RAND_MAX / 0.5f);
			p.velocity.x = (float)(rand()) / (float)(RAND_MAX / 5.0f) - 10.0f;
			p.velocity.y = (float)(rand()) / (float)(RAND_MAX / 8.0f) - 4.0f;

			p.pos.x = rand() % 90 - 30;
			if (p.velocity.y > 0)
				p.pos.y = (float)(rand()) / (float)(RAND_MAX / 12.0f);
			else if (p.velocity.y < 0)
				p.pos.y = -(float)(rand()) / (float)(RAND_MAX / 12.0f);
			else
				p.pos.y = 0.0f;
			p.pos.z = 1;

			p.acceleration.x = 0;
			p.acceleration.y = 0;
			particle.push_back(p);
		}
	}

	// Portal
	if (type == 4)
	{
		for (unsigned int i = 0; i < nrOfParticles; i++)
		{
			Particles p;
			p.type = type;
			p.lifeTime = (float)(rand()) / (float)(RAND_MAX / 0.3f);
			float r = (float)(rand()) / (float)(RAND_MAX / (DirectX::XM_2PI));
			float k = (float)(rand()) / (float)(RAND_MAX);
			p.pos.x = cos(r) * k - 0.8f;
			p.pos.y = sin(r) * k;
			p.pos.z = 0;

			//int negx = rand() % 2;
			//if (!negx)
			//	negx = -1;
			//int negy = rand() % 2;
			//if (!negy)
			//	negy = -1;
			float speed = 15.0f;
			p.velocity.x = speed * cos(r);
			p.velocity.y = speed * sin(r);
			p.acceleration.x = 0;
			p.acceleration.y = 0;
			particle.push_back(p);
		}
	}

	HRESULT hr;
	this->nrOfParticles = nrOfParticles;
	ID3D11Buffer* vertexBuffer = nullptr;

	D3D11_BUFFER_DESC partbufferDesc;
	memset(&partbufferDesc, 0, sizeof(partbufferDesc));
	partbufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_VERTEX_BUFFER;
	partbufferDesc.Usage = D3D11_USAGE_DEFAULT;
	partbufferDesc.ByteWidth = sizeof(Particles) * nrOfParticles;
	partbufferDesc.StructureByteStride = sizeof(Particles);
	partbufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = particle.data();

	hr = device->CreateBuffer(&partbufferDesc, &vertexData, &vertexBuffer);

	//COMPUTE SHADER
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof(uavDesc));
	uavDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
	uavDesc.Buffer.NumElements = nrOfParticles * sizeof(Particles) / 4;

	hr = device->CreateUnorderedAccessView(vertexBuffer, &uavDesc, &particleUAV);
	geometry = new Geometry(vertexBuffer, 0, nullptr);

	// Constant Buffer
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(constantBufferData);
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &constantBufferData;

	hr = device->CreateBuffer(&bufferDesc, &data, &constantBuffer);
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create constant buffer in Particles");
	}
	// Ett stycke hård kod
	constantBufferData.lifeTime = 50;
	constantBufferData.reset = true;
	particleCounter = 0;
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

void Particle::Reset()
{
	this->constantBufferData.position.x = 0;
	this->constantBufferData.position.y = 0;
	this->constantBufferData.position.z = 0;
}

void Particle::UpdateParticle(float deltaTime, ID3D11DeviceContext* deviceContext, ID3D11ComputeShader* computeShader)
{
	this->constantBufferData.deltaTime = deltaTime;

	//deviceContext->UpdateSubresource(constantBuffer, 0, NULL, &this->constantBufferData, 0, 0);
	UpdateConstantBuffer(deviceContext);
	ID3D11UnorderedAccessView* pUAV[] = { particleUAV };
	deviceContext->CSSetUnorderedAccessViews(0, 1, pUAV, 0);
	deviceContext->CSSetShader(computeShader, nullptr, 0);
	deviceContext->CSSetConstantBuffers(0, 1, &constantBuffer);

	deviceContext->Dispatch((nrOfParticles / 64) + 1, 1, 1);
	pUAV[0] = nullptr;
	deviceContext->CSSetUnorderedAccessViews(0, 1, pUAV, 0);
	deviceContext->CSSetShader(nullptr, nullptr, 0);
}

void Particle::SetLifeTime(float time)
{
	constantBufferData.lifeTime = time;
}

void Particle::Render(ID3D11DeviceContext* deviceContext)
{
	ID3D11Buffer* vertexBuffer = geometry->GetVertexBuffer();
	UINT stride = sizeof(Particles);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	if (changeTexture == false)
		SRV = material1->GetTexture();
	else
		SRV = material2->GetTexture();
	deviceContext->PSSetShaderResources(0, 1, &SRV);
	deviceContext->Draw(nrOfParticles, 0);
}

void Particle::UpdateConstantBuffer(ID3D11DeviceContext* deviceContext)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	deviceContext->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	memcpy(mappedResource.pData, &constantBufferData, sizeof(ConstantBufferData));
	deviceContext->Unmap(constantBuffer, 0);
}

void Particle::SetMaterial(Material* material)
{
	this->material1 = material;
}


Particle::~Particle()
{
	if (particleUAV)
		particleUAV->Release();
	if (geometry->GetVertexBuffer())
		geometry->GetVertexBuffer()->Release();
	delete geometry;

	if (SRV)
		SRV->Release();
}

void Particle::UpdateColor(bool renderParticles, const Color color, Particle* explosionParticles, std::vector<Material*> materials)
{
	if (color == Color::GREEN)
	{
		this->changeTexture = false;
		if (renderParticles == false || explosionParticles->constantBufferData.reset == true)
			explosionParticles->SetMaterial(materials[Color::GREEN]);
	}

	else if (color == Color::RED)
	{
		this->changeTexture = true;
		if (renderParticles == false || explosionParticles->constantBufferData.reset == true)
			explosionParticles->SetMaterial(materials[Color::RED]);
	}

	else if (color == Color::YELLOW)
	{
		this->changeTexture = false;
		if (renderParticles == false || explosionParticles->constantBufferData.reset == true)
			explosionParticles->SetMaterial(materials[Color::YELLOW]);
	}

	else if (color == Color::BLUE)
	{
		this->changeTexture = true;
		if (renderParticles == false || explosionParticles->constantBufferData.reset == true)
			explosionParticles->SetMaterial(materials[Color::BLUE]);
	}
}