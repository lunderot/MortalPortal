#include "MapItem.h"
MapItem::MapItem(Geometry* geometry, Material* material, Shader* shader, objectType type,
	Color color, MapItem* child, 
	DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 velocity, DirectX::XMFLOAT3 angleVelocity, DirectX::XMFLOAT3 acceleration, DirectX::XMFLOAT3 rotation, DirectX::XMFLOAT3 scale)
	: Entity(geometry, material, shader, position, velocity, angleVelocity, acceleration, rotation, scale)
{
	this->type = type;
	this->colorState = color;
	this->child = child;
};

void MapItem::Update(float deltaTime)
{
	Entity::Update(deltaTime);
	if (child)
	{
		child->SetPosition(GetPosition());
	}
	
	if (position.x < -32)
	{
		alive = false;
	};
}

Color MapItem::GetColor() const
{
	return colorState;
}
void MapItem::SetColor(Color color)
{
	colorState = color;
}

MapItem* MapItem::GetChild()
{
	return child;
}