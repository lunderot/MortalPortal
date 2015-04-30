#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "Geometry.h"
#include "Material.h"

struct buttonPoint
{
	DirectX::XMFLOAT2 position;
	DirectX::XMFLOAT2 UV;
};

struct ButtonScale
{
	DirectX::XMMATRIX scale;
};


class Button
{
	friend class StartMenu;
	friend class PauseMenu;
	friend class RestartMenu;
protected:
	DirectX::XMFLOAT2 position;
	DirectX::XMFLOAT2 scale;
	Material* material;

public:
	Button(DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 sc, Material* mat);
	virtual bool isClicked() = 0;
	~Button();
};