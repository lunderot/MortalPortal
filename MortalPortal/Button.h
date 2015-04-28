#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "Geometry.h"
#include "Material.h"

struct buttonPoints
{
	DirectX::XMFLOAT2 position[4];
	DirectX::XMFLOAT2 UV[4];
};

class Button
{
	friend class Menu;
protected:
	DirectX::XMFLOAT2 position;
	DirectX::XMFLOAT2 scale;
	Geometry* geometry;
	Material* material;

public:
	Button(DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 sc, Geometry* geo, Material* mat);
	virtual void isClicked() = 0;
	~Button();
};