#include "StartButton.h"
#include <iostream>
StartButton::StartButton(LevelGenerator* lvlGen, EntityHandler* entityHandler,
	Player* player1, Player* player2,
	DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 scale, Material* material)
	: Button(position, scale, material)
{
	this->lvlGen = lvlGen;
	this->entityHandler = entityHandler;
	this->player1 = player1;
	this->player2 = player2;
}

bool StartButton::IsClicked()
{
	entityHandler->KillAllMapItems();
	lvlGen->Reset();
	player1->Reset();
	player2->Reset();

	return false;
}

StartButton::~StartButton()
{

}