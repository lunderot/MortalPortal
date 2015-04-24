#pragma once
#include <DirectXMath.h>

class CollisionSphere
{
public:
	DirectX::XMFLOAT3 position;
	float radius;
public:
	CollisionSphere(DirectX::XMFLOAT3 position, float radius);
	~CollisionSphere();
};

