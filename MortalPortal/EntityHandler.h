#pragma once

#include <vector>

#include "Entity.h"
#include "Shader.h"

class EntityHandler
{
	std::vector<Entity*> entities;
public:
	EntityHandler();
	~EntityHandler();

	void Update(float deltaTime);
	virtual void Render(ID3D11DeviceContext* deviceContext, Shader* shader);

	void Add(Entity* entity);
};

