#pragma once
#include "Button.h"

class QuitButton : public Button
{
public:

	QuitButton(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 scale, Material* material);
	~QuitButton();

	bool isClicked();


};