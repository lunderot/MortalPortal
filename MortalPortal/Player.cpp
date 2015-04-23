#include "Player.h"

#include <iostream>

Player::Player(ID3D11Device* device,
	Importer* importer,
	unsigned int meshID,
	DirectX::XMFLOAT3 position,
	DirectX::XMFLOAT3 velocity,
	DirectX::XMFLOAT3 acceleration,
	DirectX::XMFLOAT3 rotation
	) : Entity(device, importer, meshID, position, velocity, acceleration, rotation)
{

}

Player::~Player()
{
}

unsigned int Player::PlayerColorState(unsigned int state)
{
	state = 5;
	std::cout << "state: " << state << std::endl;
	return state;
}