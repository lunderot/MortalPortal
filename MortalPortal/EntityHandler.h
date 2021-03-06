#pragma once

#include <vector>
#include <map>
#include <list>
#include <iostream>

#include "Entity.h"
#include "Shader.h"
#include "AssetHandler.h"
#include "Player.h"
#include "MapItem.h"
#include "BackgroundAsset.h"
#include "PowerupIndicator.h"
#include "AudioMaster.h"
#include "D3DHandler.h"
#include "OrthoHudShader.h"

#include "Digit.h"

class EntityHandler
{
	std::map<Shader*, std::list<Entity*>> entities;
	std::vector<Shader*> shaderOrder;
public:
	EntityHandler();
	~EntityHandler();

	void Update(float deltaTime, AudioMaster &aMaster);
	virtual void Render(ID3D11DeviceContext* deviceContext, D3DHandler* d3dHandler);
	void KillAllMapItems();

	void Add(Entity* entity);
private:
	void HandleCollision(Player* player, Entity* entity2, std::string name, AudioMaster &aMaster);
};
