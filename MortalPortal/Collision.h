#pragma once
#include <vector>

#include "CollisionSphere.h"

class Collision
{
private:
	std::vector<CollisionSphere> spheres;
public:
	Collision();
	~Collision();

	void Add(CollisionSphere collisionSphere);
};

