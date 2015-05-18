#pragma once
#include "Button.h"
#include "EntityHandler.h"
#include "Player.h"

class StartButton : public Button
{
private:
	EntityHandler* entityHandler;
	Player* player1;
	Player* player2;
public:

	StartButton(EntityHandler* entityHandler, Player* player1, Player* player2, DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 scale, Material* material);
	~StartButton();

	bool IsClicked();

};