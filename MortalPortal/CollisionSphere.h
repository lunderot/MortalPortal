#pragma once
#include <DirectXMath.h>
#include <string>

class CollisionSphere
{
public:
	DirectX::XMFLOAT3 position;
	float radius;
	std::string name;
public:
	CollisionSphere(DirectX::XMFLOAT3 position, float radius, std::string name);
	~CollisionSphere();
};

bool IsSpheresColliding(const CollisionSphere& sphere1, const CollisionSphere& sphere2, const DirectX::XMMATRIX& world1, const DirectX::XMMATRIX& world2);

