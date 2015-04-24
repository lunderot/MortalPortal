#include "CollisionSphere.h"


CollisionSphere::CollisionSphere(DirectX::XMFLOAT3 position, float radius)
{
	this->position = position;
	this->radius = radius;
}


CollisionSphere::~CollisionSphere()
{
}
