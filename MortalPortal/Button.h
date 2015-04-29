#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "Geometry.h"
#include "Material.h"



class Button
{
	friend class Menu;
protected:
	DirectX::XMFLOAT2 position;
	DirectX::XMFLOAT2 scale;
	Material* material;

public:
	Button(DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 sc, Material* mat);
	virtual bool isClicked() = 0;
	~Button();
};