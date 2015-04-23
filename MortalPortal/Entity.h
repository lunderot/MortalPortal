#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

#include "Importer.h"

class Entity
{
	friend class EntityHandler;
protected:
	Entity(); // ---

	ID3D11Buffer* vertexBuffer;
	unsigned int vertexCount;

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 velocity;
	DirectX::XMFLOAT3 acceleration;
	DirectX::XMFLOAT3 rotation;

public:
	Entity(ID3D11Device* device,
		DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 velocity = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 acceleration = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 rotation = DirectX::XMFLOAT3(0, 0, 0)
		);
	Entity(ID3D11Device* device,
		Importer* importer,
		unsigned int meshID,
		DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 velocity = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 acceleration = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 rotation = DirectX::XMFLOAT3(0, 0, 0)
		);
	virtual ~Entity();

	virtual void Update(float deltaTime);

	void SetPosition(DirectX::XMFLOAT3 position);
	void SetVelocity(DirectX::XMFLOAT3 velocity);
	void SetAcceleration(DirectX::XMFLOAT3 acceleration);
	void SetRotation(DirectX::XMFLOAT3 rotation);


	DirectX::XMFLOAT3 GetPosition() const;
	DirectX::XMFLOAT3 GetVelocity() const;
	DirectX::XMFLOAT3 GetAcceleration() const;
	DirectX::XMFLOAT3 GetRotation() const;

	ID3D11Buffer* GetVertexBuffer() const;
	unsigned int GetVertexCount() const;
};

