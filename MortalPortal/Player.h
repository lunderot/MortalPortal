#pragma once
#include "Entity.h"
class Player :
	public Entity
{
public:
	Player(ID3D11Device* device,
		Importer* importer,
		unsigned int meshID,
		DirectX::XMFLOAT2 position = DirectX::XMFLOAT2(0, 0),
		DirectX::XMFLOAT2 velocity = DirectX::XMFLOAT2(0, 0),
		DirectX::XMFLOAT2 acceleration = DirectX::XMFLOAT2(0, 0));
	
		//mange
		unsigned int colorState = 0;

		unsigned int PlayerColorState(unsigned int state);

	~Player();
};

