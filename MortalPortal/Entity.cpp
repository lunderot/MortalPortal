#include "Entity.h"

using namespace DirectX;

Entity::Entity(ID3D11Device* device,
	Importer* importer,
	unsigned int meshID,
	DirectX::XMFLOAT2 position,
	DirectX::XMFLOAT2 velocity,
	DirectX::XMFLOAT2 acceleration
	)
{
	this->position = position;
	this->velocity = velocity;
	this->acceleration = acceleration;

	

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = importer->getMeshVertexCount(meshID) * sizeof(VertexPositionTexCoordNormalBinormalTangent);

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = importer->getMesh(meshID);
	HRESULT hr = device->CreateBuffer(&bufferDesc, &data, &vertexBuffer);
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create vertex buffer");
	}

	vertexCount = importer->getMeshVertexCount(meshID);
}


Entity::~Entity()
{
	if (vertexBuffer)
	{
		vertexBuffer->Release();
	}
}

void Entity::Update(float deltaTime)
{
	velocity.x += acceleration.x * deltaTime;
	velocity.y += acceleration.y * deltaTime;

	position.x += velocity.x * deltaTime;
	position.y += velocity.y * deltaTime;
}

void Entity::SetPosition(XMFLOAT2 position)
{
	this->position = position;
}

void Entity::SetVelocity(XMFLOAT2 velocity)
{
	this->velocity = velocity;
}

void Entity::SetAcceleration(XMFLOAT2 acceleration)
{
	this->acceleration = acceleration;
}

XMFLOAT2 Entity::GetPosition() const
{
	return position;
}

XMFLOAT2 Entity::GetVelocity() const
{
	return velocity;
}

XMFLOAT2 Entity::GetAcceleration() const
{
	return acceleration;
}

ID3D11Buffer* Entity::GetVertexBuffer() const
{
	return vertexBuffer;
}

unsigned int Entity::GetVertexCount() const
{
	return vertexCount;
}
