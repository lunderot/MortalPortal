#include "Background.h"

using namespace DirectX;

Background::Background(Geometry* geometry, Material* material, Shader* shader,
	DirectX::XMFLOAT3 position,
	DirectX::XMFLOAT3 velocity,
	DirectX::XMFLOAT3 acceleration,
	DirectX::XMFLOAT3 rotation) : Entity(geometry, material, shader, position, velocity, acceleration, rotation)
{
	
}


Background::~Background()
{
	
};

void Background::Update(float deltaTime)
{
	Entity::Update(deltaTime);
	if (position.x > 35)
	{
		position.x = -35;
	}
}