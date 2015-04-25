#pragma once
#include "Entity.h"

class MapItem : public Entity
{
public:

	enum objectType{Comet, Player1_Crystal1, Player1_Crystal2, Player2_Crystal1, Player2_Crystal2, BackgroundAsset};

	MapItem(Geometry* geometry, Material* material, Shader* shader,	objectType type,
		DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 velocity = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 acceleration = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 rotation = DirectX::XMFLOAT3(0, 0, 0)
		);

	objectType type;

	void killObject();

	void Update(float deltaTime);
};
