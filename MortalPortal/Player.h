#pragma once
#include "Entity.h"
#include "Shader.h"

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
//#include <DDSTextureLoader.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")



class Player :
	public Entity
{
public:

	Player(Geometry* geometry, Material* material, Shader* shader,
		DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 velocity = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 acceleration = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 rotation = DirectX::XMFLOAT3(0, 0, 0)
		);



	//unsigned int PlayerColorState(unsigned int state);

	~Player();

	//mange
	unsigned int colorState = 0;
	

};

