#pragma once
#include "Entity.h"
class Player :
	public Entity
{
public:
	Player(Geometry* geometry,
		DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 velocity = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 acceleration = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 rotation = DirectX::XMFLOAT3(0, 0, 0)
		);
	
		//mange
		unsigned int colorState = 0;

		unsigned int PlayerColorState(unsigned int state);

	~Player();
};

