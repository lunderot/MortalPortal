#include "Entity.h"

using namespace DirectX;

//---
Entity::Entity()
{

}

Entity::Entity(ID3D11Device* device,
	DirectX::XMFLOAT3 position,
	DirectX::XMFLOAT3 velocity,
	DirectX::XMFLOAT3 acceleration,
	DirectX::XMFLOAT3 rotation)
{
	vertexBuffer = nullptr;
	vertexCount = 0;

	this->position = position;
	this->velocity = velocity;
	this->acceleration = acceleration;
	this->rotation = rotation;

}
Entity::Entity(ID3D11Device* device,
	Importer* importer,
	unsigned int meshID,
	DirectX::XMFLOAT3 position,
	DirectX::XMFLOAT3 velocity,
	DirectX::XMFLOAT3 acceleration,
	DirectX::XMFLOAT3 rotation
	)
{
	this->position = position;
	this->velocity = velocity;
	this->acceleration = acceleration;
	this->rotation = rotation;



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
	velocity.z += acceleration.z * deltaTime;

	XMVECTOR positionVec = XMLoadFloat3(&position);
	XMVECTOR velocityVec = XMLoadFloat3(&velocity);
	XMVECTOR rotationQuat = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&rotation));

	rotationQuat = XMQuaternionInverse(rotationQuat);

	XMVectorScale(velocityVec, deltaTime);

	velocityVec = XMVector3Rotate(velocityVec, rotationQuat);

	positionVec = XMVectorAdd(positionVec, velocityVec);

	XMStoreFloat3(&position, positionVec);
}

void Entity::SetPosition(XMFLOAT3 position)
{
	this->position = position;
}

void Entity::SetVelocity(XMFLOAT3 velocity)
{
	this->velocity = velocity;
}

void Entity::SetAcceleration(XMFLOAT3 acceleration)
{
	this->acceleration = acceleration;
}

void Entity::SetRotation(DirectX::XMFLOAT3 rotation)
{
	this->rotation = rotation;
}

XMFLOAT3 Entity::GetPosition() const
{
	return position;
}

XMFLOAT3 Entity::GetVelocity() const
{
	return velocity;
}

XMFLOAT3 Entity::GetAcceleration() const
{
	return acceleration;
}

XMFLOAT3 Entity::GetRotation() const
{
	return rotation;
}

ID3D11Buffer* Entity::GetVertexBuffer() const
{
	return vertexBuffer;
}

unsigned int Entity::GetVertexCount() const
{
	return vertexCount;
}



