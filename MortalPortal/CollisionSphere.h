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

bool IsSpheresColliding(const CollisionSphere& sphere1, const CollisionSphere& sphere2, const DirectX::XMMATRIX& world1, const DirectX::XMMATRIX& world2);

