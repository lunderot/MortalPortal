#include "PowerupIndicator.h"

using namespace DirectX;

PowerupIndicator::PowerupIndicator(Geometry* geometry, Material* material, Shader* shader,
	DirectX::XMFLOAT3 position,
	DirectX::XMFLOAT3 velocity,
	DirectX::XMFLOAT3 angleVelocity,
	DirectX::XMFLOAT3 acceleration,
	DirectX::XMFLOAT3 rotation,
	DirectX::XMFLOAT3 scale) : Entity(geometry, material, shader, position, velocity, angleVelocity, acceleration, rotation, scale)
{

}


PowerupIndicator::~PowerupIndicator()
{

};

void PowerupIndicator::UpdatePosition(DirectX::XMFLOAT3 position)
{
	this->playerPos = position;
}

void PowerupIndicator::Update(float deltaTime)
{


	Entity::Update(deltaTime);
	position.x = playerPos.x - 1;
	position.y = playerPos.y;

	position.x -= position.x / 32 * 2;
	position.y -= position.y / 15 * 1.5;
	//if (position.x > scale.x * 2)
	//{
	//	position.x = -(scale.x * 2 - 2);
	//}
}