#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

#include "Importer.h"

class Entity
{
	friend class EntityHandler;
protected:
	ID3D11Buffer* vertexBuffer;
	unsigned int vertexCount;

	DirectX::XMFLOAT2 position;
	DirectX::XMFLOAT2 velocity;
	DirectX::XMFLOAT2 acceleration;

public:
	Entity(ID3D11Device* device,
		Importer* importer,
		unsigned int meshID,
		DirectX::XMFLOAT2 position = DirectX::XMFLOAT2(0, 0),
		DirectX::XMFLOAT2 velocity = DirectX::XMFLOAT2(0, 0),
		DirectX::XMFLOAT2 acceleration = DirectX::XMFLOAT2(0, 0)
		);
	virtual ~Entity();

	virtual void Update(float deltaTime);

	void SetPosition(DirectX::XMFLOAT2 position);
	void SetVelocity(DirectX::XMFLOAT2 velocity);
	void SetAcceleration(DirectX::XMFLOAT2 acceleration);


	DirectX::XMFLOAT2 GetPosition() const;
	DirectX::XMFLOAT2 GetVelocity() const;
	DirectX::XMFLOAT2 GetAcceleration() const;

	ID3D11Buffer* GetVertexBuffer() const;
	unsigned int GetVertexCount() const;
};

