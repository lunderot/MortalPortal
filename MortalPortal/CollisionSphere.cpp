#include "CollisionSphere.h"


CollisionSphere::CollisionSphere(DirectX::XMFLOAT3 position, float radius)
{
	this->position = position;
	this->radius = radius;
}


CollisionSphere::~CollisionSphere()
{
}

bool IsSpheresColliding(const CollisionSphere& sphere1, const CollisionSphere& sphere2, const DirectX::XMMATRIX& world1, const DirectX::XMMATRIX& world2)
{
	DirectX::XMVECTOR pos1 = DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&sphere1.position), world1);
	DirectX::XMVECTOR pos2 = DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&sphere2.position), world2);
	DirectX::XMFLOAT3 finalPos1;
	DirectX::XMFLOAT3 finalPos2;
	DirectX::XMStoreFloat3(&finalPos1, pos1);
	DirectX::XMStoreFloat3(&finalPos2, pos2);

	//If the distance between the center points of the spheres are less than the sum of the spheres radiuses, there is a collision
	return sqrt(pow(finalPos2.x - finalPos1.x, 2) + pow(finalPos2.y - finalPos1.y, 2) + pow(finalPos2.z - finalPos1.z, 2)) < sphere1.radius + sphere2.radius;
}
