#include "Player.h"

#include <iostream>

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

unsigned int Player::PlayerColorState(unsigned int state)
{
	state = 5;
	std::cout << "state: " << state << std::endl;
	return state;
}