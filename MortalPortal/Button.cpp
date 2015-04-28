#include "Button.h"

Button::Button(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 scale, Geometry* geometry, Material* material)
{
	this->position = position;
	this->scale = scale;

	buttonPoints points;
	points.position[0] = DirectX::XMFLOAT2(-0.5f, -0.5f);
	points.position[1] = DirectX::XMFLOAT2(-0.5f, 0.5f);
	points.position[2] = DirectX::XMFLOAT2(0.5f, -0.5f);
	points.position[3] = DirectX::XMFLOAT2(0.5f, 0.5f);

	points.UV[0] = DirectX::XMFLOAT2(1.0f, 1.0f);
	points.UV[1] = DirectX::XMFLOAT2(1.0f, 1.0f);
	points.UV[2] = DirectX::XMFLOAT2(1.0f, 1.0f);
	points.UV[3] = DirectX::XMFLOAT2(1.0f, 1.0f);

	this->geometry = geometry;
	this->material = material;
}

Button::~Button()
{
	geometry->~Geometry();
	material->~Material();
}