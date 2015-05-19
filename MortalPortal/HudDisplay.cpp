#include "HudDisplay.h"


HudDisplay::HudDisplay(Geometry* geometry, Material* material, Shader* shader,
	DirectX::XMFLOAT3 position,
	DirectX::XMFLOAT3 velocity,
	DirectX::XMFLOAT3 angleVelocity,
	DirectX::XMFLOAT3 acceleration,
	DirectX::XMFLOAT3 rotation,
	DirectX::XMFLOAT3 scale)
	:Entity(geometry, material, shader, position, velocity, angleVelocity, acceleration, rotation, scale)
{
}


HudDisplay::~HudDisplay()
{
}
