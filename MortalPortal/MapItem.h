#pragma once
#include "Entity.h"

class MapItem : public Entity
{
public:

	enum objectType{Comet, Crystal, PowerUp};

	MapItem(Geometry* geometry, Material* material, Shader* shader,	objectType type,
		Color color, MapItem* child = nullptr,
		DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 velocity = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 angleVelocity = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 acceleration = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 rotation = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 scale = DirectX::XMFLOAT3(1, 1, 1)
		);

	objectType type;

	void Update(float deltaTime);

	Color GetColor() const;
	void SetColor(Color color);

	MapItem* GetChild();
private:
	Color colorState;
	MapItem* child;
};
