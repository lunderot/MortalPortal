#include "Collision.h"


Collision::Collision()
{
}


Collision::~Collision()
{
}

void Collision::Add(CollisionSphere collisionSphere)
{
	spheres.push_back(collisionSphere);
}
