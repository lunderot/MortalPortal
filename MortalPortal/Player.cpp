#include "Player.h"


Player::Player(ID3D11Device* device,
	Importer* importer,
	unsigned int meshID,
	DirectX::XMFLOAT2 position,
	DirectX::XMFLOAT2 velocity,
	DirectX::XMFLOAT2 acceleration) : Entity(device, importer, meshID, position, velocity, acceleration)
{
}


Player::~Player()
{
}

