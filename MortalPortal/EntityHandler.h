#pragma once

#include <vector>
#include <map>
#include <iostream>

#include "Entity.h"
#include "Shader.h"
#include "AssetHandler.h"
#include "Player.h"
#include "MapItem.h"
#include "BackgroundAsset.h"
#include "AudioMaster.h"

class EntityHandler
{
	std::map<Shader*, std::vector<Entity*>> entities;
public:
	EntityHandler();
	~EntityHandler();

	void Update(float deltaTime, AudioMaster &aMaster);
	virtual void Render(ID3D11DeviceContext* deviceContext);
	void KillAllMapItems();

	void Add(Entity* entity);
private:
	void HandleCollision(Player* player, Entity* entity2, AudioMaster &aMaster);
};
