#pragma once

#include <vector>

#include "Entity.h"
#include "Shader.h"
#include "Importer.h"

class EntityHandler
{
	std::vector<Entity*> entities;
public:
	EntityHandler();
	~EntityHandler();

	void Update(float deltaTime);
	virtual void Render(ID3D11DeviceContext* deviceContext);

	void Add(Entity* entity);
};

