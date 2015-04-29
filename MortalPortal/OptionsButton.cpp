#include "OptionsButton.h"
#include <iostream>
OptionsButton::OptionsButton(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 scale, Material* material)
	: Button(position, scale, material)
{

}

bool OptionsButton::isClicked()
{
	std::cout << "OPTIONS!" << std::endl;
	return true;
}

OptionsButton::~OptionsButton()
{

}