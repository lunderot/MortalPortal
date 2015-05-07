#include "BackgroundAsset.h"

BackgroundAsset::BackgroundAsset(Geometry* geometry, Material* material, Shader* shader,
	DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 velocity, DirectX::XMFLOAT3 angleVelocity, DirectX::XMFLOAT3 acceleration, DirectX::XMFLOAT3 rotation, DirectX::XMFLOAT3 scale)
	: Entity(geometry, material, shader, position, velocity, angleVelocity, acceleration, rotation, scale)
{

};

void BackgroundAsset::Update(float deltaTime)
{
	Entity::Update(deltaTime);
	if (position.x < -30)
	{
		alive = false;
	};
}
