#include "Player.h"

#include <iostream>

Player::Player(Geometry* geometry,
	DirectX::XMFLOAT3 position,
	DirectX::XMFLOAT3 velocity,
	DirectX::XMFLOAT3 acceleration,
	DirectX::XMFLOAT3 rotation
	) : Entity(geometry, position, velocity, acceleration, rotation)
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