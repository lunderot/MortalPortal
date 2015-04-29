#include "Button.h"

Button::Button(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 scale, Material* material)
{
	this->position = position;
	this->scale = scale;

	this->material = material;
}

Button::~Button()
{

}