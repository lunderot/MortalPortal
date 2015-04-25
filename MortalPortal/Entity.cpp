#include "Entity.h"

using namespace DirectX;

Entity::Entity(Geometry* geometry, Material* material, Shader* shader,
	DirectX::XMFLOAT3 position,
	DirectX::XMFLOAT3 velocity,
	DirectX::XMFLOAT3 acceleration,
	DirectX::XMFLOAT3 rotation)
{
	this->geometry = geometry;
	this->material = material;
	this->shader = shader;

	this->position = position;
	this->velocity = velocity;
	this->acceleration = acceleration;
	this->rotation = rotation;

	this->alive = true;
}	


Entity::~Entity()
{

}

void Entity::Update(float deltaTime)
{
	velocity.x += acceleration.x * deltaTime;
	velocity.y += acceleration.y * deltaTime;
	velocity.z += acceleration.z * deltaTime;

	position.x += velocity.x * deltaTime;
	position.y += velocity.y * deltaTime;
	position.z += velocity.z * deltaTime;
}

Geometry* Entity::GetGeometry() const
{
	return geometry;
}

Material* Entity::GetMaterial() const
{
	return material;
}

Shader* Entity::GetShader() const
{
	return shader;
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

void Entity::SetAlive(bool alive)
{
	this->alive = alive;
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

bool Entity::GetAlive() const
{
	return alive;
}
