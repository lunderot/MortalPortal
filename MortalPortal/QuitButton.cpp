#include "QuitButton.h"
#include <iostream>
QuitButton::QuitButton(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 scale, Material* material)
	: Button(position, scale, material)
{

}

bool QuitButton::IsClicked()
{
	PostQuitMessage(0);
	return true;
}

QuitButton::~QuitButton()
{

}