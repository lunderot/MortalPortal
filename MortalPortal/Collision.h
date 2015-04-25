#pragma once
#include <vector>

#include "CollisionSphere.h"

class Collision
{
public:
	std::vector<CollisionSphere> spheres;
public:
	Collision();
	~Collision();
};

