#include "MapItem.h"
MapItem::MapItem(Geometry* geometry, Material* material, Shader* shader, objectType type, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 velocity, DirectX::XMFLOAT3 angleVelocity, DirectX::XMFLOAT3 acceleration, DirectX::XMFLOAT3 rotation, DirectX::XMFLOAT3 scale)
	: Entity(geometry, material, shader, position, velocity, angleVelocity, acceleration, rotation, scale)
{
	this->type = type;
};

void MapItem::killObject()
{
	alive = false;
}

void MapItem::Update(float deltaTime)
{
	Entity::Update(deltaTime);
	if (position.x < -30)
	{
		alive = false;
	};
}