#include "Background.h"

using namespace DirectX;

Background::Background(Geometry* geometry, Material* material, Shader* shader,
	DirectX::XMFLOAT3 position,
	DirectX::XMFLOAT3 velocity,
	DirectX::XMFLOAT3 angleVelocity,
	DirectX::XMFLOAT3 acceleration,
	DirectX::XMFLOAT3 rotation, 
	DirectX::XMFLOAT3 scale) : Entity(geometry, material, shader, position, velocity, angleVelocity, acceleration, rotation, scale)
{
	
}


Background::~Background()
{
	
};

void Background::Update(float deltaTime)
{
	Entity::Update(deltaTime);

	if (position.x > scale.x * 2)
	{
		position.x = -(scale.x * 2 - 1);
	}

	//if (position.x > scale.x * 2)
	//{
	//	position.x = -(scale.x * 2 - 2);
	//}
}