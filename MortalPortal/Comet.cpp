#include "Comet.h"
Comet::Comet(Geometry* geometry, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 velocity ,DirectX::XMFLOAT3 acceleration ,DirectX::XMFLOAT3 rotation)
	: Entity(geometry, position, velocity, acceleration, rotation)
{

};

void Comet::Update(float deltaTime)
{
	if (position.x < -30)

}